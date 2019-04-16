#ifndef __ESPMODULEBASE_H__
#define __ESPMODULEBASE_H__

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

// #include <WiFi.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>

// if DEBUGGING is enabled
#ifdef DEBUG_ESP_PORT
  #define DEBUG_PRINT(...) { DEBUG_ESP_PORT.printf(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_ESP_PORT.printf(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

typedef enum{
  
  /* state from ESPBootstrap library */
  STATE_READY,                  //0 
	STATE_WIFI_CONNECT,           //1 
	STATE_WIFI_ACTIVE,            //2 
	STATE_ACCESS_POINT_CONNECT,   //3 
	STATE_ACCESS_POINT_ACTIVE,    //4 
	STATE_SLEEP,                  //5 

  /* ESP specific states */
  STATE_BOOTED,                 //6
  STATE_REQUEST_CONFIG,         //7 
  STATE_RESPONSE_CONFIG_DETAILS,//8 : used by server
  STATE_ACTIVE,                 //9
  STATE_PASSIVE,                //10: if ESP is to be kept idle but connected to WiFi [Future Scope]
  STATE_OTA_AVAILABLE,          //11
  STATE_INIT_OTA                //12

  // STATE_DATA_RECORD = 100 // used to notify that ESP is sending data to store on server
  // STATE_DATA_ACK = 101 // Acknowledgement sent by server indicating data received

} ESPState;


class ESPModuleBase{
  private:
  
          char* espMac;

          char* apSsid;
		      char* apPassword;

          ESP8266WiFiMulti WiFiMulti;
          WebSocketsClient webSocket;

          char* serverHost;
          char* serverUrl;
		      char* serverPort;

          char* otaServerHostUrl;
		      char* otaServerPort;

          int samplingFrequency;
          int numOfPins;
          int* espPinNumbers;
          SensorBase* sensors;
          ESPState state;
          ESPState nextState;

          // variables for timer
          unsigned long startMillis;
          unsigned long currentMillis;

  public:
          ESPModuleBase();
          ~ESPModuleBase();

          void setESPState(ESPState state);
          ESPState getESPState();
          int getSamplingFrequency(int freq);
          void setSamplingFrequency();
          
          bool startWifi();
          void stopWifi();
          void printWifiStatus();

          void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
          void initWebsocket();

          boolean begin();
          void readAndSendData(); // USER DEFINES THIS if else conditions
          virtual void setupESP();
          void loopEvent(); // read data based on sampling freq

          inline boolean cycleComplete(){
            currentMillis = millis();  //get the current "time"
            if (currentMillis - startMillis >= samplingFrequency*1000)  //test whether the period has elapsed
            {
              //if so, record reading
              readAndSendData();
              
              //IMPORTANT to update the start time of the moment recording was done.
              startMillis = currentMillis;  
            }
          }

};


#endif __ESPMODULEBASE_H__ /* __ESPMODULEBASE_H__ */

