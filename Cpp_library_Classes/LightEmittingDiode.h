#ifndef LIGHTEMITTINGDIODE_H
#define LIGHTEMITTINGDIODE_H
//include classes Sensor and Debuggable

#include "SensorBase.h"

class LightEmittingDiode : public SensorBase{
    public:
        inline LightEmittingDiode( byte connectionPin ) { 
            modelType = "LED";
            category = 0;
            numOfPins = 1; 
            pinLabels =  new string[numOfPins] {"SIGNAL"}; 

            pins = new byte[numOfPins] { connectionPin };
            power = 0;
            init();
        }
        inline LightEmittingDiode( byte userPin ,byte inputPower) { 
            modelType = "LED";
            category = 0;
            numOfPins = 1; 
            pinLabels =  new string[numOfPins] {"SIGNAL"}; 

            pins = new byte[numOfPins] { connectionPin }; 
            power = inputPower;
            init();
        }

        inline void setPower(byte p){ power = p; }
        inline byte getPower(){ return power; }

        //read function must be implemented
        virtual void setHigh(); 
        virtual void setLow();
    private:
        byte power;
};

#endif