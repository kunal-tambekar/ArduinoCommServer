
#ifndef _ESPMODULEBASE_H
#define _ESPMODULEBASE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <ESP8266BootstrapLite.h>
#include <Hash.h>
#include "SensorBase.h"

typedef enum {
    STATE_BOOTING = 6,                //6
    STATE_REQUEST_CONFIG = 7,         //7
    STATE_RESPONSE_CONFIG = 8,        //8   : used by server
    STATE_ACTIVE = 9,                 //9
    STATE_PASSIVE = 10,               //10  : if ESP is to be kept idle but connected to WiFi [Future Scope]
    STATE_OTA_AVAILABLE = 11,         //11  : passed by server
    STATE_INIT_OTA = 12,              //12  : triggers OTA
    STATE_DATA_RECORD = 13,           //13  : used to notify that ESP is sending data to store on server
    STATE_DATA_ACK = 101              //101 : Acknowledgement sent by server indicating data received
} ESPStatus;

const int RESET_WIFI_BUTTON_PIN = 2;

class EspModuleBase {

protected:

    ESP8266WiFiMulti WiFiMulti;
    WebSocketsClient webSocket;

    char *host;
    char *url;
    int port;

    String otaHost;
    String otaPort;

    char *ssid;
    char *password;

    char *ssid_ap;
    char *password_ap;
    String token;

    ESP8266BootstrapLite bootlite;

    int sampling_freq;

// variables for timer
    unsigned long startMillis;
    unsigned long currentMillis;
    ESPStatus estatus;

public:



    void setWifiCredentials(char *wifiName, char *pwd) {
        this->ssid = wifiName;
        this->password = pwd;

    }

    void setServerConfig(char *h, char *u, int p) {
        this->host = h;
        this->url = u;
        this->port = p;
    }

    void setOtaServerConfig(String h, String p) {
        this->otaHost = h;
        this->otaPort = p;
    }

    void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {

        switch (type) {
            case WStype_DISCONNECTED: {
                Serial.printf(" Web Socket Disconnected!\n");
            }
                break;
            case WStype_CONNECTED: {
                Serial.printf("Connected to url: %s\n", payload);
                // send message to server when Connected
                String jsonStr = "{\"code\": 7 ,\"mac\":\"" + WiFi.macAddress() + "\"}";
                webSocket.sendTXT(jsonStr);
            }
                break;
                // on receiving message from server
            case WStype_TEXT: {
                // initial config to handle JSON data
                const size_t jsonCapacity =
                        3 * JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2) + 3 * JSON_OBJECT_SIZE(5) + 260;
                DynamicJsonBuffer jsonBuffer(jsonCapacity);
                JsonObject &root = jsonBuffer.parseObject(payload);

                int state = root["state"];

                root.printTo(Serial);
                Serial.printf("Parsing RESPONSE \n");
                switch (state) {
                    case STATE_RESPONSE_CONFIG: {
                        /* code for setting ESP config */
                        String str_sampling_freq = root["sampling_freq"];
                        sampling_freq = str_sampling_freq.toInt();

                        for (int s = 0; s < root["sensors"].size(); s++) {

                            JsonArray &ja = root["sensors"][s]["spins"];

                            String type = root["sensors"][s]["type"];
                            PinConfig pconf[ja.size()];

                            for (int i = 0; i < ja.size(); i++) {
                                String strEpin = ja[i]["epin"];
                                pconf[i].esp_pin = strEpin.toInt();

                                String sp = ja[i]["sensor_pin"];
                                pconf[i].setSPin(sp);

                                byte m;
                                String pmode = ja[i]["pin_mode"];
                                if (pmode == "OUTPUT") {
                                    pconf[i].pin_mode = OUTPUT;
                                } else if (pmode == "INPUT") {
                                    pconf[i].pin_mode = INPUT;
                                } else {
                                    pconf[i].pin_mode = INPUT_PULLUP;
                                }

                                String mk = ja[i]["misc_key"];
                                pconf[i].setMK(mk);

                                String mv = ja[i]["misc_val"];
                                pconf[i].setMV(mv);
                            }

                            SensorBase::getSensorByType(type)->configureSensor(pconf);
                        }
                        startMillis = millis();
                        estatus = STATE_ACTIVE;
                    }
                        break;

                    case STATE_OTA_AVAILABLE: {
                        /* code for OTA Update */
                        estatus = STATE_INIT_OTA;
                        Serial.println("Will perform OTA NOW");
                        webSocket.disconnect();
                        delay(1000);
                        bootlite.update(WiFi.macAddress());
                    }
                        break;

                    case STATE_DATA_ACK: {
                        Serial.printf("ACK received\n");
                    }
                        break;

                    default:// not reachable
                    {
                        Serial.printf("UNKNOWN state\n");
                    }
                        break;
                }
                // Optional: send ACK to server to notify config received successfully
                break;
            }
        }
    }

    EspModuleBase(){
        ssid_ap = "espdualmode";
        password_ap = "someoneqwerty";
        token = "5c50e740c332c60013c76948";

        ESP8266BootstrapLite bootlite(ssid_ap, password_ap, RESET_WIFI_BUTTON_PIN);

        estatus = STATE_BOOTING;
        sampling_freq = 60;
    }

    ~EspModuleBase(){ }

    void setupESP() {

        Serial.begin(115200);
        Serial.setDebugOutput(true);

        Serial.println("IN SETUP");
        Serial.println(WiFi.macAddress());



        WiFiMulti.addAP(ssid, password);

        while (WiFiMulti.run() != WL_CONNECTED) {
            delay(100);
        }

        bootlite.enableOTAUpdates(otaHost, otaPort, token);

        //  bool success = bootLite.begin();
        Serial.println("Wifi connected");
        webSocket.begin(host, port, url);  // server address, port and URL
        webSocket.onEvent(webSocketEvent); // event handler

        // try again in 5 sec if connection failed
        webSocket.setReconnectInterval(5000);



        startMillis = millis();  //get the current "time"
    }

    void eloop() {
        //  ESPBootstrapError err =  bootLite.bootstrap();

        if (WiFi.status() == WL_CONNECTED) {

            // Disconnect websockets before initiating OTA update
            if (estatus != STATE_INIT_OTA)
                webSocket.loop();
            else
                webSocket.disconnect();

            if (estatus) {
                currentMillis = millis();  //get the current "time"

                if (currentMillis - startMillis >= sampling_freq * 100) {

                    const size_t capacity = 2 * JSON_OBJECT_SIZE(3);
                    DynamicJsonBuffer jsonBuffer(capacity);

                    JsonObject &root = jsonBuffer.createObject();
                    root["mac"] = WiFi.macAddress();
                    root["code"] = 13;

                    SensorBase *itr = SensorBase::head;
                    while (itr != nullptr) {
                        const size_t cap = JSON_ARRAY_SIZE(2) + 2 * JSON_OBJECT_SIZE(2) + 60;
                        DynamicJsonBuffer jsonBuffer(cap);
                        JsonObject &json = jsonBuffer.createObject();

                        Serial.println(itr->getType());
                        itr->retrieveSensorData(json);

                        JsonArray &rt = json["data"];

                        JsonObject &data = root.createNestedObject("data");

                        for (int x = 0; x < rt.size(); x++) {
                            const char *k = rt[x]["key"];
                            int v = rt[x]["value"];
                            data[k] = v;
                        }
                        String jsonresp;
                        root.printTo(jsonresp);
                        webSocket.sendTXT(jsonresp);
                        delay(100);
                    }
                    startMillis = currentMillis;  //IMPORTANT to update the start time of the moment recording was done.
                }

            } else {
                Serial.println("Device not yet configured.");
            }
        } else {
            delay(100);
            Serial.println("Wifi not active yet.");
        }
    }


};


#endif _ESPMODULEBASE_H
