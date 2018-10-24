var express = require('express');
var router = express.Router();
var db =  require('./database');

router.get('/', function(req, res) {
  res.redirect((req.query.filter)?"/esp/home?filter="+req.query.filter:"/esp/home");
});

router.get('/home', function(req, res, next) {
  let filter=-1;
  // filter by Status 0 - unconfigured ; 1 - Online ; 2 - Offline
  if(req.query.filter && Number.isInteger(Number.parseInt(req.query.filter))){
    filter = req.query.filter % 3;
  }

  db.getAllEsps(filter,result=>{
    console.log(JSON.stringify(result));
    res.render('esp_home',{title:'ESP Home', esps : result})
  },err=>{
    res.send("ESP HOME JS ERROR while fetching Sensor info :"+ JSON.stringify(err))
  });

});

router.get('/add', function(req, res, next) {
  res.render('esp_add',{title:'Add ESP'});
});

router.get('/configure', function(req, res, next) {
  db.getEspByMac(req.query.mac,
    result=>{
      res.render('esp_configure',{title:'Configure ESP', esp : result});
      
    },
    err=>{
      res.send("ESP MODIFY JS ERROR while fetching Sensor info :"+ JSON.stringify(err));
    });
});

router.get('/modify', function(req, res, next) {
  db.getEspByMac(req.query.mac,
    result=>{
      res.render('esp_modify',{title:'Modify ESP', esp : result });
    },
    err=>{
      res.send("ESP MODIFY JS ERROR while fetching Sensor info :"+ JSON.stringify(err));
    });
  
});

module.exports = router;
