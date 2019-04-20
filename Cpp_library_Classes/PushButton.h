#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

//include any required classes here along with SensorBase
#include "SensorBase.h"
#include <Arduino.h>

class PushButton : public SensorBase{
    public:
        inline PushButton( byte btnPin, byte ctrlPin ) {
            modelType = "PUSH_BUTTON";
            category = 1;
            numOfPins = 1; 
            pinLabels =  new String[numOfPins] {"PIN"};

            pins = new byte[numOfPins] { btnPin };
            controlPin = ctrlPin;
            init();
        }

        void init(){
            pinMode(pins[0], INPUT);
        }
        //readAndSendData function must be implemented for PushButton
        String readAndSendData(){
            readAndTakeAction();
            String val = (digitalRead(pins[0])== HIGH)?"0":"1";
            String jsonStr =  "\"pressed\" :"+val+"";
            return jsonStr;
            // these values will be extracted from this json and stored as
            // { 'parameter' : 'pressed', 'value' : 1 } in the Mongo DB if pressed
            // { 'parameter' : 'pressed', 'value' : 0 } in the Mongo DB when not pressed

        }


        void readAndTakeAction(){
            // read the state of the pushbutton value:
            int buttonState = digitalRead(pins[0]);

            // check if the pushbutton is pressed,
            // if it is, the buttonState is HIGH: pressed
            if (buttonState == HIGH) {
                setHigh();
            } else {
                setLow();
            }

        }

    void setHigh(){
        digitalWrite(controlPin, HIGH); // set controlPin HIGH/on:
    }

    void setLow(){
        digitalWrite(controlPin, LOW); // set controlPin LOW/off
    }

    private:
        byte controlPin;
};

#endif