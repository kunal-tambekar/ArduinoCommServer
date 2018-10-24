var express = require('express');
var router = express.Router();
var db = require('./database');
const url = require('url');

/* GET api base url. */
router.get('/', function(req, res, next) {
  var qs = req.query;
  console.log(JSON.stringify(req.headers));
  
  res.json({message:'Please enter specific url for the data you need >> '+Object.keys(qs),
            url: JSON.stringify(qs)
          });
});

/* POST api base url. */
router.post('/', function(req, res, next) {
    const param1 = req.body.param1;
    res.json({message:'Please enter specific url for the data you need '+param1});
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
    let typ = req.body.pin_type;
    req.body.pin_type = [];
    req.body.pin_type.push(typ);
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

/*ESP */

router.get('/esp',function(req,res){
  
  let filter=-1;
  // filter by Status 0 - unconfigured ; 1 - Online ; 2 - Offline
  if(req.query.filter && Number.isInteger(Number.parseInt(req.query.filter))){
    filter = req.query.filter % 3;
  }

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
    let typ = req.body.pin_type;
    req.body.pin_type = [];
    req.body.pin_type.push(typ);
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


module.exports = router;