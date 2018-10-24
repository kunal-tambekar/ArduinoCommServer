#include "ESPModuleBase.h"

ESPModuleBase::ESPModuleBase(){
  this->_esp_mac = WiFi.macAddress();
  this->state = STATE_BOOTING;
}

ESPModuleBase::~ESPModuleBase(){

}

void ESPModuleBase::setESPState(ESPState state){
  this->state = state;
}

ESPState ESPModuleBase::getESPState(){
  return this->state;
}

void ESPModuleBase::setSamplingFrequency(int freq){
  this->samplingFrequency = freq;
}

int ESPModuleBase::getSamplingFrequency(){
  return this->samplingFrequency;
}

void ESPModuleBase::printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

bool ESPModuleBase::startWifi(){
  //  Connecting to WiFi

  WiFi.disconnect();
  int counter = 10;
  WiFiMulti.addAP(this->_ap_ssid, this->_ap_password);

  while(WiFiMulti.run() != WL_CONNECTED && counter>=0) {
    delay(200);
    counter--;
  }

  return (counter > 0)?true:false;
}

void ESPModuleBase::stopWifi(){

  DEBUG_PRINTLN("Shutting down wifi\n");

	delay(1000);
	if(WiFi.isConnected()){

	  WiFi.softAPdisconnect();

	  WiFi.disconnect();

	  delay(1000);
  }
}

void ESPModuleBase::initWebsocket(){

  // server address, port and URL
  webSocket.begin(host, port, url);

  // event handler for websocket communication
  webSocket.onEvent(this->webSocketEvent);

  // NOTE: If theres a need to use HTTP Basic Authorization [OPTIONAL <remove if not needed> ]
//  webSocket.setAuthorization("user", "Password");

  // Retry in 5 sec if connection failed
  webSocket.setReconnectInterval(5000);

}

void ESPModuleBase::webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
    case WStype_DISCONNECTED:
      DEBUG_PRINT(this->_esp_mac);
      DEBUG_PRINT("Websocket Disconnected!\n");
      break;
    case WStype_CONNECTED: {
      DEBUG_PRINT(this->_esp_mac);
      DEBUG_PRINT("Connected to url: %s\n", payload);
      // Log message to debug when Connected
      //TODO: create JSON message with STATE_REQUEST_CONFIG


      webSocket.sendTXT("Connected and Messaged");
    }
      break;
    case WStype_TEXT:
      DEBUG_PRINT(">> get text: %s\n", payload);
      //TODO: get the updated state from the request/response from server
      processServerMessage(payload);


      //TODO: Code for switch case logic for the STATE DIAGRAM will go
      switch(state){
        case STATE_READY:
          break;
        case STATE_WIFI_CONNECT:
          break;
        case STATE_WIFI_ACTIVE:
          break;
        case STATE_ACCESS_POINT_CONNECT:
          break;
        case STATE_ACCESS_POINT_ACTIVE:
          break;
        case STATE_SLEEP:
          break;

        case STATE_BOOTING:
          break;
        case STATE_REQUEST_CONFIG:
          break;
        case STATE_RESPONSE_CONFIG_DETAILS:
          break;
        case STATE_CONFIG_UPDATED:
          break;
        case STATE_ACTIVE:
          break;
        case STATE_ONLINE:
          break;
        case STATE_OFFLINE:
          break;
        case STATE_OTA_AVAILABLE:
          break;
        case STATE_INIT_OTA:
          break;
        default:
          break;
      }


      break;
  }

}
