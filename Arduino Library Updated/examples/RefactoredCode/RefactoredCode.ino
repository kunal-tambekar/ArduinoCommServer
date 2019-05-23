#include<Arduino.h>
#include <WebSockets.h>
#include <WebSocketsClient.h>
#include "EspModuleBase.h"

#include "LED.h"
#include "PushButton.h"

LED l;
PushButton p;

/* --v-- TO CONFIGURE BY USER --v-- */

char *sid ="AndroidKT";
char *pwd = "passpass";

char *host= "192.168.43.82";
char *url = "/";
int port  = 3000;

String otaHost = "192.168.43.82";
String otaPort = "3001";

String token = "5c50e740c332c60013c76948";

/* --^-- TO CONFIGURE BY USER --^-- */

EspModuleBase esp(token);

void setup() {
  
  esp.setWifiCredentials(sid,pwd);
  esp.setServerConfig(host,url,port);
  esp.setOtaServerConfig(otaHost,otaPort);
  esp.setupESP();
  esp.webSocket.onEvent(webSocketEvent);
}

void loop() {
  esp.eloop();
}
