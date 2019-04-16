#include "Arduino.h"
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <WebSocketsClient.h>
#include <Hash.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

const char* host = "10.0.0.54";
const char* url= "/";
const int port = 3000;

const char* ssid = "100Font@3K2.4"; //"AndroidKT";
const char* password = "FONT@100";  //"passpass";

SensorBase* sensors = new SensorBase[2];

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf(" Web Socket Disconnected!\n");
      break;
    case WStype_CONNECTED:
      Serial.printf("Connected to url: %s\n", payload);
      // send message to server when Connected
      String jsonStr =  "{\"code\":7,\"mac\":\""+WiFi.macAddress()+"\"}";
      // NOTE <reference code>: Code to conver to char* IF NEEDED [not needed in this case]
      // char json[50];
      // jsonStr.toCharArray(json, sizeof(json));
      webSocket.sendTXT(jsonStr);
      break;

    case WStype_TEXT:
      const size_t capacity = 5*JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(8) + 150;
      DynamicJsonBuffer jsonBuffer(capacity);
      JsonObject& root = jsonBuffer.parseObject(payload);
      const int* espPinNumbers = new int[2] {0,2};

      int state = root["state"]; // 8
      int nextState; // 8 --> 9  ||  11 --> 12

      root.printTo(Serial);
      
      switch (state)
      {
        case 8:
          /* code for setting ESP config */
          nextState = 9;
          const char* sampling_freq = root["sampling_freq"]; // "120"

          // const char* sensors0 = root["sensors"][0]; // "LED"
          // const char* sensors1 = root["sensors"][1]; // "PUSH_BUTTON"

          // const char* sensor_pin0 = root["sensor_pin"][0]; // "POWER"
          // const char* sensor_pin1 = root["sensor_pin"][1]; // "PIN"
          
          // const char* pin_mode0 = root["pin_mode"][0]; // "OUTPUT"
          // const char* pin_mode1 = root["pin_mode"][1]; // "INPUT"
          
          // const char* misc_keys0 = root["misc_keys"][0]; // "POWER"
          // const char* misc_keys1 = root["misc_keys"][1]; // "CONTROL"
          
          // const char* misc_val0 = root["misc_val"][0]; // "1"
          // const char* misc_val1 = root["misc_val"][1]; // "2"

          for(int i = 0 ; i < 2 ; i++){
            switch(root["sensors"][i]){
              case "LED":
                sensors[i] = new LightEmittingDiode(espPinNumbers[i]);
                if(root["misc_val"][i]=="POWER"){
                  sensors[i].serPower((root["misc_val"][i]=="1")?1:0);
                }
              break;
              case "PUSH_BUTTON":

                if(root["misc_val"][i]=="CONTROL"){
                  string ctrl = ""+root["misc_val"][i];
                  sensors[i] = new PushButton(espPinNumbers[i],ctrl.toInt());
                }
              break;
              case "DHT11":

              break;
              default:
              // remember to init all expected sensors, do no  leave for default
              break;
            }

          }

          break;
        case 11:
          /* code for OTA Update */
          nextState = 12;
          const char* otaServerHostUrl = root["ota_hosturl"]; 
          const char* ota_port = root["ota_port"]; 
          break;
        case 13:
          /* code for DATA_ACK */

          break;
        default:

          break;
      }

      // send ACK to server to notify config recieved successfully
      break;
  }

  // check for sampling frequency duration and send data
  

}

void setup() {

  Serial.begin(115200);
  Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  
  for(uint8_t t = 4; t > 0; t--) {
    Serial.printf("IN SETUP >>> BOOTING %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFiMulti.addAP(ssid, password);

  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }

  webSocket.begin(host, port, url);  // server address, port and URL
  webSocket.onEvent(webSocketEvent); // event handler

  // try again in 5 sec if connection failed
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
}