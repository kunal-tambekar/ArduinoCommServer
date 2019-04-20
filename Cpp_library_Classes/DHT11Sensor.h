#ifndef DHT11SENSOR_H
#define DHT11SENSOR_H

//include any required classes here along with SensorBase
#include <dht.h>
#include "SensorBase.h"
#include <Arduino.h>

//NOTE: not all sensor implementaions may need CPP file

class DHT11Sensor : public SensorBase{
    public:
        inline DHT11Sensor( byte signalPin ) {
            modelType = "DHT11";
            category = 1;
            numOfPins = 1; 
            pinLabels =  new String[1] {"SIGNAL"};
            pins = new byte[1] { signalPin };

            init(); // might as well be skipped
        }

        void init(){ /* do nothing here as DHT library handles initialization  */ }

        String readAndSendData(){
            // Read data and creat json response
            int sts = DHT.read11(pins[0]);
            String jsonStr = "";
            switch (sts) {
                case DHTLIB_OK: {
                        Serial.print("OK,\t");
                        String h = String(DHT.humidity, DEC);
                        String t = String(DHT.temperature, DEC);
                        jsonStr += "\"humidity\": " + h + ", \"temperature\": " + t + "";
                    }
                    break;
                case DHTLIB_ERROR_CHECKSUM: 
                    Serial.print("Checksum error,\t"); 
                    break;
                case DHTLIB_ERROR_TIMEOUT: 
                    Serial.print("Time out error,\t"); 
                    break;
                default: 
                    Serial.print("Unknown error,\t"); 
                    break;
            }

            return jsonStr;
        }
        
    private:
        dht DHT;
};

#endif