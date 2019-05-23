#include<Arduino.h>
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include "EspModuleBase.h"

// Initializing the global sensor list
SensorBase* SensorBase::head = nullptr;

// Code to setup and handle Websocket connection
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED: {
          Serial.printf(" Web Socket Disconnected!\n");
        }
        break;
    case WStype_CONNECTED: {
          esp.connectAndRequestConfig();
        }
        break;
    case WStype_TEXT: {
          esp.handleConfigResponse(payload);
        }
        break;
    }
}
