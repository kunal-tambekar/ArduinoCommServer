#ifndef LIGHTEMITTINGDIODE_H
#define LIGHTEMITTINGDIODE_H
//include classes Sensor and Debuggable

#include "SensorBase.h"
#include <Arduino.h>

class LightEmittingDiode : public SensorBase{
    public:
        inline LightEmittingDiode( int connectionPin ) {
            modelType = "LED";
            category = 0;
            numOfPins = 1; 
            pinLabels =  new String[numOfPins] {"SIGNAL"};

            pins = new byte[numOfPins] { connectionPin };
            power = 0;
            init();
        }

        inline LightEmittingDiode( byte connectionPin ,byte inputPower) {

            modelType = "LED";
            category = 0;
            numOfPins = 1; 
            pinLabels =  new String[numOfPins] {"SIGNAL"};
            pins = new byte[numOfPins] { connectionPin };

            init();
            power = inputPower;
            if(power==1) {
                setHigh();
            }else {
                setLow();
            }
        }

        void init(){
            pinMode(pins[0], OUTPUT);
        }

        // these values will be extracted from this json and stored as
        // { 'parameter' : 'power', 'value' : 1 } in the Mongo DB if On
        // { 'parameter' : 'power', 'value' : 0 } in the Mongo DB when Off
        String readAndSendData(){

            String val = (digitalRead(pins[0])==HIGH)?"0":"1";
            String jsonStr =  "\"power\" : "+val+"";
            return jsonStr;

        }

        inline void setPower(int p){ power = p; }
        inline byte getPower(){ return power; }

        //read function must be implemented
        int setHigh(){
            Serial.printf("TURN LED ON on pin #%d\n",pins[0]);
            digitalWrite(pins[0], HIGH);   // turn the LED on (HIGH is the voltage level)
            delay(1000);
        }

        int setLow(){
            Serial.printf("TURN LED OFF on pin #%d\n",pins[0]);
            digitalWrite(pins[0], LOW);   // turn the LED off (LOW is the voltage level)
            delay(1000);
        }

    private:
        byte power;
};

#endif