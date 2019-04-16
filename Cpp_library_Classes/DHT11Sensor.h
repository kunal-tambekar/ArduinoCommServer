#ifndef DHT11SENSOR_H
#define DHT11SENSOR_H

//include any required classes here along with SensorBase
#include <dht.h>
#include "SensorBase.h"

//NOTE: not all sensor implementaions may need CPP file

class DHT11Sensor : public DHT11Sensor{
    public:
        inline DHT11Sensor( byte connectionPin ) { 
            modelType = "DHT11";
            category = 1;
            numOfPins = 1; 
            pinLabels =  new string[1] {"SIGNAL"}; 
            pins = new byte[1] { connectionPin };

            init(); // might as well be skipped
        }

        inline void init(){ /* do nothing as  DHT library handles initialization  */ }

        inline string readAndSendData(){
            // Read data and creat json response
            int sts = DHT.read11(dhtPin);
            switch (sts)
            {
                case DHTLIB_OK:  
                    Serial.print("OK,\t"); 
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
            string jsonStr =  "{\"humidity\": "+DHT.humidity+", \"temperature\": "+ DHT.temperature +" }";
            return jsonStr;
        }
        
    private:
        dht DHT;
};

#endif