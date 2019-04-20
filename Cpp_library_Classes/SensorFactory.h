//
// Created by Kunal Tambekar on 2019-04-15.
//

#ifndef ARDUINOCOMM_SENSORFACTORY_H
#define ARDUINOCOMM_SENSORFACTORY_H

#include "SensorBase.h"

class SensorFactory {
public:
    static SensorBase* createSensor(String sensorType);
};


#endif //ARDUINOCOMM_SENSORFACTORY_H
