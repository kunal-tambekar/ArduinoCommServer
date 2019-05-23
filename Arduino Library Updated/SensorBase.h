#ifndef __SENSORBASE_H__
#define __SENSORBASE_H__

#include <Arduino.h>
#include <ArduinoJson.h>
#include "PinConfig.h"


class SensorBase {

    private:
        SensorBase* next;

    public:
        static SensorBase* head;

        SensorBase* getNext(){
            return next;
        }

        SensorBase(){
            next = head;
            head = this;
        }

        String getType() const {
            return type;
        };

        virtual void configureSensor(const PinConfig* pins) = 0;
        virtual void retrieveSensorData(JsonObject& jo) = 0;

        static SensorBase* getSensorByType(String t){

            SensorBase *itr = head;

            while (itr!=nullptr){
                if(itr->type==t){
                    return itr;
                }
                itr = itr->next;
            }

            return nullptr;
        }


    protected:
        String type;
        int numOfPins;

};


#endif __SENSORBASE_H__
