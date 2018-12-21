#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>
#include <ArduinoJson.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

const char* host = "10.0.0.54"; // IP of the host
const char* url= "/";
const int port = 3000;

const char* ssid = "Wifi@2.4";  //100Font@3K2.4
const char* password = "password";

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("X Web Socket Disconnected!\n");
      break;
    case WStype_CONNECTED: {
        Serial.printf("Connected to url: %s\n", payload);
        // send message to server when Connected
        // In case of the Echo server this same message will be sent back
        webSocket.sendTXT("Connected and Messaged");
      }
      break;
    case WStype_TEXT:
      StaticJsonBuffer<200> jsonBuffer;
      char json[] = payload;
      JsonObject& root = jsonBuffer.parseObject(json);

      // Test if parsing succeeds.
      if (!root.success()) {
        Serial.println("parseObject() failed");
        return;
      }
      
      const char* opcode = root["opcode"];

      switch(opcode){
        case "RES_CONFIG": 
          const char* sensor = root["sensor"];
          int sampling_freq = root["sampling_freq"];
          Serial.printf(">> parsed text: %i\n", sampling_freq);

      }

      

      break;
  }

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

//  Connecting to WiFi
  WiFiMulti.addAP(ssid, password); 

//  WiFi.disconnect();
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
  
  webSocket.begin(host, port, url);  // server address, port and URL

  webSocket.onEvent(webSocketEvent); // event handler

// use HTTP Basic Authorization this is optional remove if not needed
//  webSocket.setAuthorization("user", "Password"); // NOT NEEDED

// try again in 5 sec if connection failed
  webSocket.setReconnectInterval(5000);

}

void loop() {
  
  webSocket.loop();
}