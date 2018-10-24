
#ifndef __SENSORBASE_H__
#define __SENSORBASE_H__

#include "Arduino.h"
#include <ArduinoJson.h>
#include "FS.h"

// if DEBUGGING is enabled
#ifdef DEBUG_ESP_PORT
  #define DEBUG_PRINT(...) { DEBUG_ESP_PORT.printf(__VA_ARGS__); }
  #define DEBUG_PRINTLN(...) { DEBUG_ESP_PORT.printf(__VA_ARGS__); }
#else
  #define DEBUG_PRINT(...) {}
  #define DEBUG_PRINTLN(...) {}
#endif

class SensorBase{
  protected:
   char* modelType;
   int numOfPins; 
   String pinLabels[]; 

  public: 
  
  SensorBase(){};
  ~SensorBase(){};

  void readData();
  void sendData();





};


#endif /* __SENSORBASE_H__ */