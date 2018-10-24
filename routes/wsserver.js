const WebSocket = require('ws');
const url = require('url');
var db = require('./database');
const constants = require('./constants');

exports.initWebSocketServer = function (server) {
  const wss = new WebSocket.Server({
    server
  });

  wss.on('connection', function connection(ws, req) {
    const location = url.parse(req.url, true);

    ws.on('message', function incoming(message) {
      var msg={};
      try{
        msg = JSON.parse(message);
      }catch(excp){
        console.warn("non JSON data received: "+message);
      }

      if(msg.client == 0 ){
        // message is from ESP
        console.log("ESP Message : " + JSON.stringify(msg));
        if (msg.code == constants.STATE_REQUEST_CONFIG) { 
          console.log("Configuration Request received");
          //check if MAC address is in the DB 
          // IF Yes : change status to online and check config table for configurations info  to send back
          // ELSE No: Add the ESP to the espDB and make it online [unconfigured]
          db.getEspByMac(msg.mac,function(doc){
            doc.state=constants.STATE_RESPONSE_CONFIG_DETAILS;
            doc.code = constants.STATE_ACTIVE; // default to Active
            ws.send(JSON.stringify(doc));
          },(err)=>{
            console.warn(err);
            
          })
          
  
        } else {

        }

      }else if(msg.client == 1 ){ 
        // message is from Web Client

        if (msg.code == "DASHBOARD_INFO") {
          // <NOTE> USE socket io for client side socket communication
        } else
        if (msg.code == "DASHBOARD_STOP") {
  
        } else
        if (msg.code == "VIEW_ESP_DATA") {
          // this will be sent from web client when it wants to view ESPs Data
          // TODO 1: Add the ws to the EspObserverMap <MAC,[ws1,ws2,ws3...]>
          // TODO 2: send this to ESP msg.code == "START_SEND_DATA" to tell esp to send data
  
        } else
        if (msg.code == "ESP_DATA") {
  
  
        } else
        if (msg.code == "CONFIGURE_ESP") {
          // called by Web client (MAY not need) as GET call can do this but keep for now
  
        } 
        
      }else {
        console.log("Unknown request received >> " + msg.code);
      }

      
      

      console.log('Server received: %s from ' + message + ' with IP %s', msg.code, req.connection.remoteAddress);

    });

    ws.on('error', function errCloseConnection(error) {
      console.log("There was an " + error);
      espObserverMap.delete(ws);

      var pos = dashboardList.indexOf(ws);
      dashboardList.splice(pos, 1);
    });

    ws.on('close', function closeConnection() {
      // if (msg!=undefined && msg.code == "STOP_SEND_DATA") {
      //     // remove the WS from the Map and pass the same message tot the ESP as well
      // }

      // console.log('DISCONNECT: Removing ' + espObserverMap.get(ws) + ' from espObserverMap whose ip was' + req.connection.remoteAddress);
      // espObserverMap.delete(ws);
    });

  });

};