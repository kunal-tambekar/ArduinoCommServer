#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

//include any required classes here along with SensorBase
#include "SensorBase.h"

class PushButton : public SensorBase{
    public:
        inline PushButton( byte btnPin, byte ctrlPin ) {
            modelType = "PUSH_BUTTON";
            category = 1;
            numOfPins = 1; 
            pinLabels =  new string[1] {"PIN"}; 

            pins = new byte[1] { btnPin };
            controlPin = ctrlPin;
            init();
        }
        
        //readAndSendData function must be implemented for PushButton
        virtual void readAndTakeAction();
        virtual void setHigh(); 
        virtual void setLow();

    private:
        byte controlPin;
};

#endif