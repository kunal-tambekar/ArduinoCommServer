#include<Arduino.h>
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include "EspModuleBase.h"

#include "LED.h"
#include "PushButton.h"

SensorBase* SensorBase::head = nullptr;

LED l;
PushButton p;

EspModuleBase esp;

void setup() {

  char *sid ="AndroidKT";
  char *pwd = "passpass";
  esp.setWifiCredentials(sid,pwd);
  esp.setServerConfig("192.168.43.82","/",3000);
  esp.setOtaServerConfig("192.168.43.82","3001");
  esp.setupESP();
  esp.webSocket.onEvent(esp.webSocketEvent);

}

void loop() {
  // put your main code here, to run repeatedly:
  esp.eloop();
}
