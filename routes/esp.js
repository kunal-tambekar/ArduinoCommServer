var express = require('express');
var router = express.Router();
var db =  require('./database');

router.get('/', function(req, res) {
  res.redirect((req.query.filter)?"/esp/home?filter="+req.query.filter:"/esp/home");
});

router.get('/home', function(req, res, next) {
  let filter=null;
  // filter by Status 0 - unconfigured ; 1 - Configured ; 2 - OTA available [not used]
  if(req.query.filter && Number.isInteger(Number.parseInt(req.query.filter))){
    filter = req.query.filter;
  }

  db.getAllEsps(filter,result=>{
    console.log(JSON.stringify(result));
    res.render('esp_home',{title:'ESP HOME', esps : result});
  },err=>{
    res.send("ESP HOME JS ERROR while fetching Sensor info :"+ JSON.stringify(err));
  });
});

router.get('/add', function(req, res, next) {
  res.render('esp_add',{title:'ADD ESP'});
});

router.get('/modify', function(req, res, next) {
  db.getEspByMac(req.query.mac,
    result=>{
      res.render('esp_modify',{title:'EDIT ESP', esp : result });
    },
    err=>{
      res.send("ESP MODIFY JS ERROR while fetching Sensor info :"+ JSON.stringify(err));
    });
  
});

router.get('/configure', function(req, res, next) {
  db.getEspByMac(req.query.mac,
    esp=>{
      db.getAllSensorNames(
        sensors=>{
          db.getConfigurationByEspId(req.query.mac,0,
            config=>{
              console.log("CONFIG: "+JSON.stringify(config))
              res.render('esp_configure',{title:'CONFIGURE ESP', esp : esp ,config:(config?config:""), sensors});
            },e=>{
            res.send("ESP CONFIGURE JS ERROR while fetching ESP config :"+ JSON.stringify(e));
          })
      },er=>{
        res.send("ESP CONFIGURE JS ERROR while fetching Sensor names :"+ JSON.stringify(er));
      })
    },
    err=>{
      res.send("ESP CONFIGURE JS ERROR while fetching ESP info :"+ JSON.stringify(err));
    });
});

router.get('/view', function(req, res, next) {

  let espId = req.query.mac;

  db.getEspByMac(espId,
    result=>{
      res.render('esp_view',{ 
        title:'VIEW ESP', 
        esp : result,
        data: { isDataAvailable : true,
                page: 1 ,
                page_size : 10
              }
      
      });
    },
    err=>{
      res.send("ESP VIEW JS ERROR while fetching info :"+ JSON.stringify(err));
    });
});

module.exports = router;
