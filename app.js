var createError = require('http-errors');
var express = require('express');
var path = require('path');
var cookieParser = require('cookie-parser');
var cors = require('cors');
var logger = require('morgan');

const wsserver = require('./routes/wsserver');
const sioserver = require('./routes/socketioserver');

var apiRouter = require('./routes/apirouter');

var indexRouter = require('./routes/index');
var sensorRouter = require('./routes/sensor');
var espRouter = require('./routes/esp');

var app = express();

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'ejs');

app.use(logger('dev'));
app.use(express.json());
app.use(express.urlencoded({
  extended: false
}));

app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

// enabling cross origin resource sharing, so that any domain can access the APIs
app.use(cors());


app.use('/', indexRouter);
app.use('/api', apiRouter);

app.use('/sensor', sensorRouter);
app.use('/esp', espRouter);
app.use('/about', function(req, res, next) {
  res.render('about', {
      head_title: 'About',
      title: 'ABOUT'

  });
});

// code to get server ip
var os = require('os');

var interfaces = os.networkInterfaces();
var addresses = [];
for (var k in interfaces) {
    for (var k2 in interfaces[k]) {
        var address = interfaces[k][k2];
        if (address.family === 'IPv4' && !address.internal) {
            addresses.push(address.address);
        }
    }
}
app.serverip = addresses;
console.log(app.serverip);

// set up mockers for testing
app.use('/mock_esp', function(req, res, next) {
  res.render('esp_mock', { server: {ip : app.serverip} });
});
app.get('/socketiotest', function(req, res){
  res.sendFile(__dirname + '/tests/sockiotest.html');
});

// catch 404 and forward to error handler
app.use(function (req, res, next) {
  next(createError(404));
});

// error handler
app.use(function (err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

var http = require('http');
var server = http.createServer(app);

//setting port
server.listen(3000,function listening(){
  console.log("ArduinoComm Listening on port "+server.address().port);
});

// sioserver.initSocketioServer(server); // not used
wsserver.initWebSocketServer(server);

module.exports = app;