const WebSocket = require('ws');
const url = require('url');
var db = require('./database');
const constants = require('./constants');

// class to store websocket connections for each device
// NOTE: since there is only one ESP module per websocket thus 1:1 mapping with MAC will work fine
class Clients{

  constructor(){
    this.clientList = {};
    this.saveClient = this.saveClient.bind(this);
    this.removeClient = this.removeClient.bind(this);
  }
  
  // connect on device ON
  saveClient(mac,ws){
    this.clientList[mac] = (ws);
  }

  // remove from map if device WS closes due to some reason
  removeClient(mac){
    this.clientList[mac] = null;
  }
  
}

const espObserverMap = new Clients();

// Use this to trigger immediate effect on ESP
exports.triggerEspEvent = function(mac,code){
 
  if(code==8){
    db.getConfigurationByEspId(mac,0,
      configJson=>{
        console.log("Called triggerEspEvent for ESP with mac address: "+mac);
        console.log("SERVER to ESP >>> websockets >>> UPADTED CONFIG: "+JSON.stringify(configJson));
        // Editing response before sending it to ESP.
        configJson.state = 8;
        configJson.code = 10;
        delete configJson._id;
        delete configJson.mac;
        delete configJson.model_type;
        delete configJson.num_of_pins;
        delete configJson.pin_label;
        
        if(espObserverMap.clientList[mac]){
          espObserverMap.clientList[mac].send(JSON.stringify(configJson));
        }
      },e=>{
      res.send("ESP CONFIGURE JS ERROR while fetching ESP config :"+ JSON.stringify(e));
    });
  }else if(code == 12){
    if(espObserverMap.clientList[mac]){
      console.log("sent OTA TRIGGER to ESP ");
      espObserverMap.clientList[mac].send("{'state':11}");      
    }
  }else{
    console.log("WS SERVER: UNKNOWN EVENT = "+code +" TRIGERRED.");
  }

}

exports.initWebSocketServer = function (server) {
 
  const wss = new WebSocket.Server({
    server
  });

  wss.on('connection', function connection(ws, req) {
    const location = url.parse(req.url, true);
    let mac ="";
    ws.on('message', function incoming(message) {
      
      var msg={};
      try{
        msg = JSON.parse(message);
      }catch(excp){
        console.warn("WARNING !!! non JSON data received: "+message);
      }
      // Manage ESP WS connections via Map like structure: 
      // Add ws to map with MAC as key Map<mac,ws>
      mac = msg.mac;
      espObserverMap.saveClient(msg.mac,ws);

      // Handle message from ESP
      if(msg){  
        console.log("ESP Message : " + JSON.stringify(msg));
        //ESP requested config from server
        if (msg.code == constants.STATE_REQUEST_CONFIG) { 
          console.log("Configuration Request received");
          //check if ESP config is present in based on MAC address
          // IF Yes : check config table for configurations info to send back
          var ip = req.connection.remoteAddress.split(":");
          ip = ip[ip.length-1];

          db.getEspByMac(msg.mac,function(doc){
              if(doc && doc.mac === msg.mac){
                console.log("Found info in ESP_collection, looking for configuration "+doc.status+":"+doc.mac);
                if( doc.status == constants.ESP_CONFIGURED){
                  db.getConfigurationByEspId(doc.mac,1,function(conf){
                    conf.state=constants.STATE_RESPONSE_CONFIG_DETAILS;
                    conf.code = constants.STATE_ACTIVE; // default to Active
                    ws.send(JSON.stringify(conf));
                  },(er)=>{
                    console.warn("Error while searching in ESP_collection: "+er);
                  });
                }else{
                  console.log(msg.mac+" not yet configured");
                }
          // ELSE No: Add the ESP to the esp_collection and make it configured status as [unconfigured]
              }else{
                // Creating new entry with Wifi Mac address of ESP8266
                let newEsp = {
                  "mac":msg.mac,
                  // IP from request to server [not much use but for keep sake]
                  "ip":ip,
                  "status":0,
                  "name":"ESP_"+msg.mac,
                  "description":"Newly added unconfigured ESP",
                  "model_type":"ESP8266",
                  "num_of_pins":2,
                  "pin_label" : [ "GPIO_0", "GPIO_2" ],
                };
                db.upsertEsp(newEsp,function(doc){
                  console.log("Added new esp to esp_collection as unconfigured: "+msg.mac);
                },er=>{
                  console.warn("Error while adding to esp_collection: "+er);
                });
              }
                
            },(err)=>{
              console.warn(err);
          });
          
        // ESP send data to server
        } else if( msg.code == constants.STATE_RECORD_DATA ){
          // Take the data and enter into the data_collection
          db.insertSensorData(msg,result =>{
            
            if(espObserverMap.clientList[mac]){
              console.log("SAVED data in data_collection "+result);
              // Optional TODO: can send ACK to ESP about received data
              espObserverMap.clientList[mac].send("{'state':13}");
              
            }

          },e=>{
            res.send("JS ERROR while storing sensor data from ESP to db :"+ JSON.stringify(e));
          });
          

        }else {

        }
      // message is from Web Client
      // }else if(msg.client == 1 ){ 
      //   if (msg.code == "DASHBOARD_INFO") {
      //     // <NOTE> USE socket io for client side socket communication if dynamic data is to be included
      //   } else
      //   if (msg.code == "DASHBOARD_STOP") {
  
      //   } else
      //   if (msg.code == "VIEW_ESP_DATA") {
      //     // this will be sent from web client when it wants to view ESPs Data
      //     // TODO 1: Add the ws to the EspObserverMap <MAC,[ws1,ws2,ws3...]>
      //     // TODO 2: send this to ESP msg.code == "START_SEND_DATA" to tell esp to send data
  
      //   } else
      //   if (msg.code == "ESP_DATA") {
  
  
      //   } else
      //   if (msg.code == "CONFIGURE_ESP") {
      //     // called by Web client (MAY not need) as GET call can do this but keep for now
  
      //   } 
        
      // }else {
      //   console.log("Unknown request received >> " + msg.code);
      }

      console.log('Server received code: %s from ' + message + ' with IP %s', ip, req.connection.remoteAddress);

    });

    ws.on('error', function errCloseConnection(error) {
      espObserverMap.removeClient(mac);
      console.log("WS ERROR: " + error +" for MAC: "+mac);
    });

    ws.on('close', function closeConnection() {
      espObserverMap.removeClient(mac);
      console.log('WS DISCONNECT: ' + mac + ' disconnected from ESP WS Server whose ip was ' + req.connection.remoteAddress);
    });

  });

};