#ifndef __ESPMODULEBASE_H__
#define __ESPMODULEBASE_H__

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
  STATE_READY,                  
	STATE_WIFI_CONNECT,           
	STATE_WIFI_ACTIVE,            
	STATE_ACCESS_POINT_CONNECT,   
	STATE_ACCESS_POINT_ACTIVE,    
	STATE_SLEEP,                  

  /* ESP specific states */
  STATE_BOOTING,
  STATE_REQUEST_CONFIG,
  STATE_RESPONSE_CONFIG_DETAILS,
  STATE_CONFIG_UPDATED,
  STATE_ACTIVE,
  // STATE_PASSIVE,  // if ESP is to be kept idle but connected to WiFi
  STATE_ONLINE,
  STATE_OFFLINE,
  STATE_OTA_AVAILABLE,
  STATE_INIT_OTA


} ESPState;


class ESPModuleBase{
  private:
  
          char* _esp_mac;

          char* _ap_ssid;
		      char* _ap_password;

          WebSocketsClient webSocket;

          char* _server_host;
          char* _server_url;
		      char* _server_port;

          char* _ota_server_host_url;
		      char* _ota_server_port;

          int samplingFrequency;
          ESPState state;

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

};


#endif __ESPMODULEBASE_H__ /* __ESPMODULEBASE_H__ */

