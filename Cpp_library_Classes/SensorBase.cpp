// There may not even be a need for this CPP file.
// all functions need to be implemented in the derived class

#include "SensorBase.h"

SensorBase::SensorBase(){ }

SensorBase::~SensorBase(){ }

virtual void init() = 0;

virtual string SensorBase::readAndSendData()=0;