
#include "Arduino.h"
#include "PushButton.h"

void PushButton::init(){
  pinMode(pins[0], INPUT);
}

string PushButton::readAndSendData(){
  string val = digitalRead(pins[0]);
  String jsonStr =  "{\"pressed\" :"+val+"}";
  return jsonStr;
  // these values will be extracted from this json and stored as 
  // { 'parameter' : 'pressed', 'value' : 1 } in the Mongo DB if HIGH/pressed
  // { 'parameter' : 'pressed', 'value' : 0 } in the Mongo DB when LOW/not pressed

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

