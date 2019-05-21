#ifndef DHT11SENSOR_H
#define DHT11SENSOR_H

//include any required classes here along with SensorBase
#include <dht.h>
#include "SensorBase.h"

class DHT11Sensor :public SensorBase{

public:
    DHT11Sensor(){
        type = "DHT11";
        numOfPins = 1;
    }

    void configureSensor(const PinConfig* pins) override {

        for (int i = 0; i < numOfPins; i++) {
            if (pins[i].sensor_pin == "SIGNAL") {
                signalPin = pins[i].esp_pin;
            }
        }
    }

    String sendData() override {
        // Read data and creat json response
        int sts = DHT.read11(signalPin);

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

protected:
    int signalPin;
    dht DHT;

};


#endif DHT11SENSOR_H
