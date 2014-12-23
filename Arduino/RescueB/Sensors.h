#include "Arduino.h"
#include "Configuration.h"

#ifndef Sensors_h
#define Sensors_h

class Sensors{
  public:
    Sensors();
    void init();
    void checkAllValues(); // Call at certain frequency 
    // Basic functions
    float getTemperature(byte address);
    float getHeading();
    
  private:
    HMC5883L compass;
};


#endif
