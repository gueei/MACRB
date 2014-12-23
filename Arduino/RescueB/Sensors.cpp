#include "Arduino.h"
#include "Configuration.h"
#include "Sensors.h"

Sensors::Sensors(){}

void Sensors::init(){
}

float Sensors::getTemperature(byte address) {
  return -1;
}

float Sensors::getHeading(){
 return -1;
}
