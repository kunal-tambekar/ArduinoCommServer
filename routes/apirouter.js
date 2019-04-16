var express = require('express');
var router = express.Router();
var db = require('./database');
const wsserver = require('./wsserver');
const url = require('url');

/* GET api base url. */
router.get('/', function(req, res, next) {
  var qs = req.query;
  console.log(JSON.stringify(req.headers));
  
  res.json({message:'GET request test: >> '+Object.keys(qs),
            url: JSON.stringify(qs)
          });
});

router.get('/temp',function(req,res){

  // nested API calls
  // db.getSensorCount( sensors =>{
  //   db.getEspCount( result =>{
  //     result.sensors = sensors.sensors;
  //     res.json(result);
  //   },
  //   er =>{
  //     res.send("Temp API ESP Error: "+er);
  //   });
  // },err=>{
  //   console.warn(err);
  //   res.send("Temp API Sensor Error: "+err);
  // });

  console.log(req.query.page+" "+req.query.size);
  db.getAllEspsPagewise(parseInt(req.query.page),parseInt(req.query.size),result =>{
    res.json(result);
  },err=>{
    res.send("Temp API ESP Error: "+err);
  });
});

/* POST api base url. */
router.post('/', function(req, res, next) {
    res.json({message:'POST req test: '+JSON.stringify(req.body)});
});

/* HOME DASHBOARD
   - ESP and sensor count */
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
  // 0 - sort by name ascending ; 1 sort by name desc ; 2 sort by model_type asc ; 3 sort by model_type desc
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

// ADD API DOES THE TASK OF UPDATE TOO : UPSERT 
// router.post('/sensor/modify',function(req,res){
//   console.log(JSON.stringify(req.body));
//   db.updateSensorWithType(req.body.model_type,req.body,(result)=>{
//     res.json(result);
//   },(err)=>{
//     console.warn(err);
//     res.send("UPDATE SENSOR API Error: "+err);
//   })
// });

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
  // NOTE: filter by Status 0 - unconfigured ; 1 - Online ; 2 - Offline [not in use]
  if(req.query.filter && Number.isInteger(Number.parseInt(req.query.filter))){
    filter = req.query.filter % 2;
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
    req.body.pin_label = [];
    req.body.pin_label.push(lbl);
    // let typ = req.body.pin_type;
    // req.body.pin_type = [];
    // req.body.pin_type.push(typ);
    console.log("\n\n\n"+JSON.stringify(req.body));
  }

  db.upsertEsp(req.body,(result)=>{
    console.log("SUCCESS: "+JSON.stringify(result));
    res.location("../../esp");
    res.redirect("../../esp");
    // res.json(result);
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
  console.log(JSON.stringify(req.body));
  db.upsertConfiguration(req.body,(result)=>{
    wsserver.triggerEspEvent(req.body.mac,8);
    console.log("SUCCESS: "+JSON.stringify(result));
    // db.updateEspWithMac({mac:req.body.mac},{mac:req.body.mac, status:1},(r)=>{
      res.location("../../esp");
      res.redirect("../../esp");
    // },(e)=>{console.log(e)});

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
  
  console.log(JSON.stringify(req.body));
  let pno=parseInt(req.body.page);
  let count = parseInt(req.body.page_size);
  let param =  req.body.parameter;
  let mac = req.body.mac;

  
  console.log("MAC= "+mac+"  Page= "+pno+" PageSize= "+count+" PARAMETER = "+param );
  db.getEspDataPagewise(mac,param,pno,count,result =>{
    // temp dummy data 
    // result = [{
    //   "timestamp": 1547179009000,
    //   "value": (Math.floor(Math.random() * 200) - 100)
    // },{
    //   "timestamp": 1547179119000,
    //   "value": (Math.floor(Math.random() * 200) - 100)
    // },{
    //   "timestamp": 1547179229000,
    //   "value": (Math.floor(Math.random() * 200) - 100)
    // },{
    //   "timestamp": 1547179339000,
    //   "value": (Math.floor(Math.random() * 200) - 100)
    // },{
    //   "timestamp": 1547179449000,
    //   "value": (Math.floor(Math.random() * 200) - 100)
    // }];
    
  
    res.json(result);
  },err=>{
    res.send("Temp API ESP Error: "+err);
  });
});


module.exports = router;