
#ifndef __SENSORBASE_H__
#define __SENSORBASE_H__

//#if ARDUINO >= 100
//  #include "Arduino.h"
//#else
//  #include "WProgram.h"
//  #include "pins_arduino.h"
//  #include "WConstants.h"
//#endif

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
    // store the model type
    char* modelType;

    // store if its a sensor or an actuator
    // 0 : "actuator" | 1 : "sensor"
    int category;

    // number of pins to connect to ESP 
    int numOfPins;

    // labels of the pins to map with the ESP pin number
    String* pinLabels;

    // ESP pin number on which the pins are mapped
    byte* pins;

  public: 

    // use to initialize and configure the sensor or actuator
    virtual void init() = 0;

    // user shall call this function in ESP sketch to trigger recording/reading data and prepare response to send to server
    virtual String readAndSendData() = 0;


};


#endif /* __SENSORBASE_H__ */