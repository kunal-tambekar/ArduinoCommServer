#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include <ESP8266BootstrapLite.h>

#include "LightEmittingDiode.h"
#include "PushButton.h"
//#include "DHT11Sensor.h"
#include <Hash.h>

// fields of the ESP Class
ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

const char* host = "192.168.43.82"; //"10.0.0.54"; //   //
const char* url = "/";
const int port = 3000;

const char* ssid =   "AndroidKT";  // "100Font@3K2.4";  //
const char* password = "passpass"; //  "FONT@100"; //
const int* espPinNumbers = new int[2] {0, 2};

char* ssid_ap = "espdualmode";
char* password_ap = "someoneqwerty";
String token = "5c50e740c332c60013c76948";
const int RESET_WIFI_BUTTON_PIN = 2;

ESP8266BootstrapLite bootlite(ssid_ap, password_ap, RESET_WIFI_BUTTON_PIN);

SensorBase *sensors[2];
int sampling_freq = 60;
const String otaHost = "192.168.43.82"; 
const String otaPort = "3001";

// variables for timer
unsigned long startMillis;
unsigned long currentMillis;
int estatus = 0 ;

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch (type) {
    case WStype_DISCONNECTED: {
        Serial.printf(" Web Socket Disconnected!\n");
      }
      break;
    case WStype_CONNECTED: {
        Serial.printf("Connected to url: %s\n", payload);
        // send message to server when Connected
        //TODO: repeat every 5 mins till config rreceived
        String jsonStr =  "{\"code\":7,\"mac\":\"" + WiFi.macAddress() + "\"}";
        webSocket.sendTXT(jsonStr);
      }
      break;
    // on receiving message from server
    case WStype_TEXT:
      {
        // initial config to handle JSON data
        const size_t capacity = 5 * JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(8) + 150;
        DynamicJsonBuffer jsonBuffer(capacity);
        JsonObject& root = jsonBuffer.parseObject(payload);


        int state = root["state"]; // 8

        root.printTo(Serial);
        Serial.printf("Parsing RESPONSE \n");
        switch (state)
        {
          case 8: {
              /* code for setting ESP config */
              String str_sampling_freq = root["sampling_freq"]; // "120"
              sampling_freq = str_sampling_freq.toInt();

              // assign int values to handle sensors in switch case
              for (int i = 0 ; i < 2 ; i++) {
                int ch = 0;
                if (root["sensors"][i] == "LED" || root["sensors"][i] == "LED_B" || root["sensors"][i] == "LED_R" )
                  ch = 1;
                else if (root["sensors"][i] == "PUSH_BUTTON" )
                  ch = 2;
                else if (root["sensors"][i] == "DHT11")
                  ch = 3;

                switch (ch) {
                  case 1: {
                      //                  Serial.printf("%d Found LED %s: %s\n",i,root["misc_keys"][i],root["misc_val"][i]);
                      if (root["misc_keys"][i] == "POWER") {
                        Serial.printf("INIT LED \n");
                        int pow = (root["misc_val"][i] == "1") ? 1 : 0;
                        LightEmittingDiode *l = new LightEmittingDiode(espPinNumbers[i], pow);
                        sensors[i] = l;
                      } else {
                        Serial.println("LED: INVALID misc kv");
                      }
                    }
                    break;
                  case 2: {
                      //                  Serial.printf("%d Found PUSH_BUTTON %s: %s\n",i,root["misc_keys"][i],root["misc_val"][i]);
                      if (root["misc_keys"][i] == "CONTROL") {
                        Serial.printf("INIT PUSH_BUTTON \n");
                        String ctrl = root["misc_val"][i];
                        int cpin = ctrl.toInt();
                        PushButton *pb = new PushButton(espPinNumbers[i], cpin );
                        sensors[i] = pb;
                      } else {
                        Serial.println("BTN: INVALID misc kv");
                      }
                    }
                    break;
                  case 3: {
                      Serial.printf("INIT DHT11 \n");
                      // DHT11Sensor *dht = new DHT11Sensor(espPinNumbers[i]);
                      //sensors[i] = dht;
                    }
                    break;
                  default: {
                      Serial.println("in DEFAULT");
                    }
                    // remember to init all expected sensors, do no  leave for default
                    break;
                }
              }
              startMillis = millis();
              estatus = 1;
            }

            break;
          case 11: {
              /* code for OTA Update */
              estatus = 2;
              Serial.println("Shoud PERFORM OTA NOWwwwwwww");
              webSocket.disconnect();
              delay(1000);
              bootlite.update(WiFi.macAddress());
            }
            break;
          case 13:
            /* code for DATA_ACK */
            {
              Serial.printf("ACK \n");
            }
            break;
          default:

            break;
        }

        // send ACK to server to notify config recieved successfully
        break;
      }
  }
}

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);

  Serial.println("IN SETUP");
  Serial.println(WiFi.macAddress());
  WiFiMulti.addAP(ssid, password);
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
  bootlite.enableOTAUpdates(otaHost,otaPort,token);

//  bool success = bootLite.begin();
  Serial.println("Wifi connected");
  webSocket.begin(host, port, url);  // server address, port and URL
  webSocket.onEvent(webSocketEvent); // event handler

  // try again in 5 sec if connection failed
  webSocket.setReconnectInterval(5000);

}

void loop() {
//  ESPBootstrapError err =  bootLite.bootstrap();

  if (WiFi.status() == WL_CONNECTED) {
//    Serial.printf("+");
    if(estatus != 2)
      webSocket.loop();
    else
      webSocket.disconnect(); 
      
    if (estatus) {
      currentMillis = millis();  //get the current "time"

      if (currentMillis - startMillis >= sampling_freq * 100) {
        //if so, record reading
        String jsonresp = "{ \"mac\": \"" + WiFi.macAddress() + "\",\"code\":13,\"data\":{";
        for (int i = 0; i < 2; i++) {
          String reading = sensors[i]->readAndSendData();
          if (i > 0 && reading != "")
            jsonresp += ",";
          jsonresp += reading;
        }

        jsonresp += "} }";
        webSocket.sendTXT(jsonresp);
        startMillis = currentMillis;  //IMPORTANT to update the start time of the moment recording was done.
      } else {
//        Serial.printf(".");
      }
    } else {
      Serial.println("Device not yet configured.");
    }
  } else {
    delay(100);
    Serial.println("Wifi not active yet.");
  }
}