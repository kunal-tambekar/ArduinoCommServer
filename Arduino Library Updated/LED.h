#ifndef __LED_H__
#define __LED_H__

#include "SensorBase.h"

class LED :public SensorBase{

public:
    LED(){
        type = "LED";
        numOfPins = 1;
    }

    void configureSensor(const PinConfig* pins) override {

        for (int i = 0; i < numOfPins; i++) {

            if (pins[i].sensor_pin == "POWER") {
                powerPin = pins[i].esp_pin;
                pinMode(powerPin, OUTPUT); // or pins[i].pin_mode

                if (pins[i].misc_key == "POWER") {
                    if (pins[i].misc_val == "0") {
                        setHigh();
                    } else {
                        setLow();
                    }
                }
            }
        }
    }

    void setHigh(){
        Serial.printf("TURN LED ON for pin %d\n",powerPin);
        digitalWrite(powerPin, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(100);
    }

    void setLow(){
        Serial.printf("TURN LED OFF for pin %d\n",powerPin);
        digitalWrite(powerPin, LOW);   // turn the LED off (LOW is the voltage level)
        delay(100);
    }

    void retrieveSensorData(JsonObject& json) override{

        JsonArray& data = json.createNestedArray("data");

        JsonObject& data_0 = data.createNestedObject();
        data_0["key"] = "power";
        String val = (digitalRead(powerPin==HIGH))?"0":"1";
        data_0["value"] = val;
    }

protected:
    int powerPin;
};

#endif __LED_H__
