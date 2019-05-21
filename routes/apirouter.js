var express = require('express');
var router = express.Router();
var db = require('./database');
const wsserver = require('./wsserver');
const url = require('url');

/* GET api base url. */
router.get('/', function(req, res, next) {
  var qs = req.query;
  console.log(JSON.stringify(req.headers));
  
  res.json({message:'GET request : >> '+Object.keys(qs),
            url: JSON.stringify(qs)
          });
});

// use this to try test out new apis
router.get('/temp',function(req,res){

 /* // nested API calls ref code
  db.getSensorCount( sensors =>{
    db.getEspCount( result =>{
      result.sensors = sensors.sensors;
      res.json(result);
    },
    er =>{
      res.send("Temp API ESP Error: "+er);
    });
  },err=>{
    console.warn(err);
    res.send("Temp API Sensor Error: "+err);
  }); */

  console.log(req.query.page+" "+req.query.size);
  db.getAllEspsPagewise(parseInt(req.query.page),parseInt(req.query.size),result =>{
    res.json(result);
  },err=>{
    res.send("Temp API ESP Error: "+err);
  });
});

/* POST api base url. */
router.post('/', function(req, res, next) {
    res.json({message:'POST req : '+JSON.stringify(req.body)});
});

/* HOME DASHBOARD : ESP and sensor count 
ESP     - 0 : Unconfigured
        - 1 : Configured
Sensor  - 0 : Actuator
        - 1 : Sensor
*/
router.get('/dashboard_info',function(req,res){
  db.getSensorCount( sensors =>{
    db.getEspCount( result =>{
      result.sensors = sensors.sensors;
      res.json(result);
    },
    er =>{
      res.send("Dashboard API ESP Error: "+er);
    });
  },err=>{
    console.warn(err);
    res.send("Dashboard API Sensor Error: "+err);
  });
});

/* SENSORS */
router.get('/sensor',function(req,res){
  
  let sort=0;
/*  0 - sort by name ascending ; 
    1 sort by name desc ; 
    2 sort by model_type asc ; 
    3 sort by model_type desc 
*/
  if(req.query.sort && Number.isInteger(Number.parseInt(req.query.sort))){
    sort = req.query.sort % 4;
  }

  db.getAllSensors(sort,(result)=>{
    console.log("SUCCESS: "+JSON.stringify(result));
    res.json({sensors:result});
  },(err)=>{
    console.warn(err);
    res.send("GET SENSOR API Error: "+err);
  });
});

router.post('/sensor/add',function(req,res){
  console.log(JSON.stringify(req.body));
  if(req.body.num_of_pins === "1"){
    let lbl = req.body.pin_label;
    req.body.pin_label = [];
    req.body.pin_label.push(lbl);
    console.log("\n\n\n"+JSON.stringify(req.body));
  }
  
  db.addNewSensor(req.body,(result)=>{
    console.log("SUCCESS: "+JSON.stringify(result));
    res.location("../../sensor");
    res.redirect("../../sensor");
  },(err)=>{
    console.warn(err);
    res.send("ADD SENSOR API Error: "+err);
  })
});

//NOTE: USE ADD API it does the task of UPDATE too a.k.a. : UPSERT 
router.post('/sensor/modify',function(req,res){
  console.log(JSON.stringify(req.body));
  db.updateSensorWithType(req.body.model_type,req.body,(result)=>{
    res.json(result);
  },(err)=>{
    console.warn(err);
    res.send("UPDATE SENSOR API Error: "+err);
  })
});

router.get("/sensor/delete/:id",function(req,res){
  db.removeSensorByType(req.params.id,
    result=>{
      res.json(result);
    },
    err=>{
      console.warn(err);
      res.send("DELETE SENSOR API Error: "+err);
    });

});

/* ESP */

router.get('/esp',function(req,res){
  
  let filter=null;
  /* NOTE: filter by Status 
    0 - Unconfigured ; 
    1 - Configured ; 
    2 - OTA [not in use] 
  */
  if(req.query.filter && Number.isInteger(Number.parseInt(req.query.filter))){
    filter = req.query.filter;
  }
  console.log("req.query.filter "+req.query.filter );
  db.getAllEsps(filter,(result)=>{
    console.log("SUCCESS: "+JSON.stringify(result));
    res.json({esps:result});
  },(err)=>{
    console.log(err);
    res.send("GET ESP API Error: "+err);
  });
});

router.post('/esp/add',function(req,res){
  console.log(JSON.stringify(req.body));
  if(req.body.num_of_pins === "1"){
    let lbl = req.body.pin_label;
    let num = req.body.pin_num;
    req.body.pin_label = [];
    req.body.pin_num = [];
    req.body.pin_label.push(lbl);
    req.body.pin_num.push(num);
    console.log("\n\n\n"+JSON.stringify(req.body));
  }

  req.body.pins = [];
  
  for(let  i = 0; i < req.body.num_of_pins ; i++){
    let pin = {
      "pin_label":req.body.pin_label[i],
      "pin_num":req.body.pin_num[i]
    }
    req.body.pins.push(pin);
  }

  // delete req.body.pin_label; 
  delete req.body.pin_num;


  db.upsertEsp(req.body,(result)=>{
    console.log("SUCCESS: "+JSON.stringify(result));
    res.location("../../esp");
    res.redirect("../../esp");
  },(err)=>{
    console.log(err);
    res.send("ADD ESP API Error: "+err);
  });
});

router.post('/esp/modify',function(req,res){
  console.log(JSON.stringify(req.body));
  db.upsertEsp(req.body,(result)=>{
    res.json(result);
  },(err)=>{
    console.log(err);
    res.send("UPDATE ESP API Error: "+err);
  });
});

router.post('/esp/configure',function(req,res){
  
  console.log("Configure ESP JSON: "+JSON.stringify(req.body));
  // Process the ESP request body before storing in the database.
  /* Convert 
  { 
    "mac" : "60:01:94:5D:91:FD",
    "model_type" : "ESP8266",
    "num_of_pins" : "2",
    "sampling_freq" : "200",

    "epin" : [ "0", "2"],
    "sensor_type" : [ "LED", "PUSH_BUTTON"],
    "sensor_pin" : [ "POWER", "PIN"],
    "pin_mode" : [ "OUTPUT", "INPUT"],
    "misc_key" : [ "POWER", "CONTROL"],
    "misc_val" : [ "1", "0"]
  }
  
  TO

  {
    "mac":"60:01:94:5D:91:FD",
    "model_type": "ESP8266",
    "num_of_pins": "2",
    "sampling_freq": "200",

    "sensors": [{
        "type": "LED",
        "spins": [{
            "epin": "0",
            "sensor_pin": "POWER",
            "pin_mode": "OUTPUT",
            "misc_key": "POWER",
            "misc_val": "1"
        }]
    }, {
        "type": "PUSH_BUTTON",
        "spins": [{
            "epin": "2",
            "sensor_pin": "PIN",
            "pin_mode": "INPUT",
            "misc_key": "CONTROL",
            "misc_val": "0"
        }]
    }]
  }

  */ 

  req.body.sensors = [];
  let ssrPinMap = {};
  for(var i = 0 ; i< req.body.sensor_type.length ; i++){
    if(!ssrPinMap[req.body.sensor_type[i]]){
      ssrPinMap[req.body.sensor_type[i]] = [];
    }
    let pin = { 
      "epin"      : req.body.epin[i],
      "sensor_pin": req.body.sensor_pin[i],
      "pin_mode"  : req.body.pin_mode[i],
      "misc_key"  : req.body.misc_key[i],
      "misc_val"  : req.body.misc_val[i]
    };

    ssrPinMap[req.body.sensor_type[i]].push(pin);

  }
  
  Object.keys(ssrPinMap).forEach(element => 
    {
      let ssr = { 
        "type" : element,
        "spins" : ssrPinMap[element] 
      }
      req.body.sensors.push(ssr);
    });

    delete req.body.epin;
    delete req.body.sensor_type;
    delete req.body.sensor_pin;
    delete req.body.pin_mode;
    delete req.body.misc_key;
    delete req.body.misc_val;
    
    // console.log("\BODY:\n"+JSON.stringify(req.body));

  db.upsertConfiguration(req.body,(result)=>{
    wsserver.triggerEspEvent(req.body.mac,8);
    console.log("SUCCESS: "+JSON.stringify(result));
    res.location("../../esp");
    res.redirect("../../esp");

  },(err)=>{
    console.log(err);
  });
});

router.get("/esp/delete/:mac",function(req,res){
  db.removeEspWithMac({mac:req.params.mac},
    result=>{
      res.json(result);
    },
    err=>{
      console.warn(err);
      res.send("DELETE ESP API Error: "+err);
    });
});

router.get("/esp/push_ota",function(req,res){
  let mac = req.query.mac;
  wsserver.triggerEspEvent(mac,12);
  res.send({status:200});
});


/* DATA */

router.get('/datafields',function(req,res){
  let mac = req.query.mac;
  db.getEspDataFields(mac,(data)=>{
    res.send(JSON.stringify(data));
  },e=>{
    res.send(JSON.stringify(e));
  });
});

router.post('/data',function(req,res){
  
  console.log("fetch esp sensor DATA POST body: "+JSON.stringify(req.body));
  let pno=parseInt(req.body.page);
  let count = parseInt(req.body.page_size);
  let param =  req.body.parameter;
  let mac = req.body.mac;

  db.getEspDataPagewise(mac,param,pno,count,result =>{
    res.json(result);
  },err=>{
    res.send("Fetch DATA READINGS API ESP Error: "+err);
  });
});


module.exports = router;