
#ifndef __SENSORBASE_H__
#define __SENSORBASE_H__

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

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
    // store the model type
    char* modelType;
    // store if its a sensor or an actuator
    // 0 : "actuator" | 1 : "sensor"
    int category;
    // number of pins to connect to ESP 
    int numOfPins; 
    // labels of the pins to map with the ESP pin number
    string pinLabels; 
    byte pins[];
  public: 
  
    SensorBase(){};
    ~SensorBase(){};
    // use to initialize and configure the sensor or actuator
    void init();
    // user shall call this function in ESP sketch to trigger recording of reading/data
    string readAndSendData();
    // user shall modify this function to send data to server
      // JSON string customization in control of user 
      //NOT needed
    void sendData();

};


#endif /* __SENSORBASE_H__ */