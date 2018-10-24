
let ESP_STATES =["STATE_READY","STATE_WIFI_CONNECT","STATE_WIFI_ACTIVE","STATE_ACCESS_POINT_CONNECT","STATE_ACCESS_POINT_ACTIVE",
	"STATE_SLEEP","STATE_BOOTING","STATE_REQUEST_CONFIG","STATE_RESPONSE_CONFIG_DETAILS","STATE_CONFIG_UPDATED","STATE_ACTIVE",
  "STATE_PASSIVE","STATE_ONLINE","STATE_OFFLINE","STATE_OTA_AVAILABLE","STATE_INIT_OTA"];

const STATE_READY = 0;                  
const STATE_WIFI_CONNECT = 1;
const STATE_WIFI_ACTIVE = 2;
const STATE_ACCESS_POINT_CONNECT = 3 ;
const STATE_ACCESS_POINT_ACTIVE = 4;
const STATE_SLEEP =5;

/* ESP specific states */
const STATE_BOOTING =6;
const STATE_REQUEST_CONFIG=7;
const STATE_RESPONSE_CONFIG_DETAILS=8;
const STATE_CONFIG_UPDATED=9;
const STATE_ACTIVE=10;
const STATE_PASSIVE =11;  // if ESP is to be kept idle but connected to WiFi
const STATE_ONLINE=12;
const STATE_OFFLINE =13;
const STATE_OTA_AVAILABLE =14;
const STATE_INIT_OTA =15;

let wsUri = "ws://localhost:3000";
let output_console;
let espState = STATE_BOOTING;
let samplingFrequency;
let esp_mac = "00:25:96:FF:FE:12:34:56";

function init() {
  output_console = document.getElementById("output_console");
  writeToScreen('<span style="color: green;">Booting... ESP_STATE: ' + ESP_STATES[espState]+'</span>');
  setTimeout(() => {
    startWifi();
    printWifiStatus();
  },1000);
}

function startWifi(){
  setTimeout(() => {
    writeToScreen('<span style="color: green;">check for ssid and password and try connecting to Wifi </span>');    
    initWebsocket();
  }, 500);
}

function printWifiStatus(){
  setTimeout(() => {
    writeToScreen('<span style="color: green;">WiFi connected </span>');
    writeToScreen('<span style="color: green;">SSID, IP Address and signal strength displayed on screen </span>');
  },1000);
}

function initWebsocket(){
  websocket = new WebSocket(wsUri);
  webSocketEvent();
}

function webSocketEvent() {
  websocket.onopen = function (evt) {
    onOpen(evt)
  };
  websocket.onclose = function (evt) {
    onClose(evt)
  };
  websocket.onmessage = function (evt) {
    onMessage(evt)
  };
  websocket.onerror = function (evt) {
    onError(evt)
  };
}

function onOpen(evt) {
  writeToScreen("WS connection established ");
  let json = { 
    client: 0,
    mac : esp_mac,
    code : STATE_REQUEST_CONFIG
   }

  doSend(json);
}

function onClose(evt) {
  writeToScreen("DISCONNECTED");
}

function onMessage(evt) {
  writeToScreen('<span style="color: blue;">RESPONSE: ' + evt.data + '</span>');
  let res = JSON.parse(evt.data);
  if(res.state == STATE_RESPONSE_CONFIG_DETAILS){
    

  }else if(res.state == STATE_CONFIG_UPDATED){

  }else{

  }


}

function onError(evt) {
  writeToScreen('<span style="color: red;">ERROR:</span> ' + evt.data);
}

function doSend(message) {
  writeToScreen('<span style="color: purple;">SENT: ' + JSON.stringify(message) +'</span>');
  websocket.send(JSON.stringify(message));
}

function writeToScreen(message) {
  var pre = document.createElement("p");
  pre.style.wordWrap = "break-word";
  pre.innerHTML = message;
  output_console.appendChild(pre);
}