#include "Arduino.h"
#include "LightEmittingDiode.h"

void LightEmittingDiode::init(){
  pinMode(pins[0], OUTPUT);
}

  // these values will be extracted from this json and stored as 
  // { 'parameter' : 'power', 'value' : 1 } in the Mongo DB if HIGH / On
  // { 'parameter' : 'power', 'value' : 0 } in the Mongo DB when LOW / Off
string LightEmittingDiode::readAndSendData(){
  string val = digitalRead(pins[0]);
  string jsonStr =  "{\"power\" : "+val+"}";
  return jsonStr;

}

int LightEmittingDiode::setHigh(){
  digitalWrite(pin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);
}

int LightEmittingDiode::setLow(){
  digitalWrite(pin, LOW);   // turn the LED off (LOW is the voltage level)
  delay(1000);
}