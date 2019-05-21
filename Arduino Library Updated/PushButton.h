#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include "SensorBase.h"

class PushButton :public SensorBase{

public:
    PushButton(){
        type = "PUSH_BUTTON";
        numOfPins = 1;
    }

    void configureSensor(const PinConfig* pins) override {
        Serial.println("CONFIGURING... PUSH BUTTON");
        for(int i =0 ; i<numOfPins;i++){
            if(pins[i].sensor_pin == "PIN"){
                connectionPin = pins[i].esp_pin;
                pinMode(connectionPin, pins[i].pin_mode); // INPUT ot INPUT_PULLUP

                Serial.printf("PBtn ON on pin %d\n",connectionPin);

                if(pins[i].misc_key=="CONTROL"){
                    controlPin = pins[i].misc_val.toInt();
                    pinMode(controlPin, OUTPUT); // as pin can be controlled only in OUTPUT mode
                    Serial.printf("PBtn Controls pin %d\n",controlPin);
                }
            }
        }
    }

//        // these values will be extracted from this json and stored as
//        // { 'pressed' : 1 } in the Mongo DB if pressed
//        // { 'pressed' : 0 } in the Mongo DB when not pressed

    void retrieveSensorData(JsonObject& json) override{

        JsonArray& data = json.createNestedArray("data");

        JsonObject& data_0 = data.createNestedObject();
        data_0["key"] = "pressed";
        String val = (digitalRead(connectionPin)== HIGH)?"0":"1";
        data_0["value"] = val;

    }

    void readAndTakeAction(){
        // read the state of the pushbutton value:
        int buttonState = digitalRead(controlPin);

        // check if the pushbutton is pressed,
        // if it is, the buttonState is HIGH: pressed
        if (buttonState == HIGH) {
            setHigh();
        } else {
            setLow();
        }
    }

    void setHigh(){
        Serial.println("set High");
        digitalWrite(controlPin, HIGH); // set controlPin HIGH/on:
    }

    void setLow(){
        Serial.println("set Low");
        digitalWrite(controlPin, LOW); // set controlPin LOW/off
    }

protected:
    int connectionPin;
    int controlPin;

};

#endif PUSHBUTTON_H