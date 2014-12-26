#include "Arduino.h"
#include "Configuration.h"
#include <i2cmaster.h>
#include <Servo.h>

#ifndef Sensors_h
#define Sensors_h

#define SensorType_Count 8

class Sensors{
  public:
    enum SensorType{
      Temp_Left = 0,
      Temp_Right = 1,
      Dist_Left = 2,
      Dist_Right = 3,
      Dist_Front = 4,
      Heading = 5,  // in Radian
      FloorGray = 6, 
      AccelTap = 7
    };
    
    Sensors();
    void init();
    void checkAllValues(); // Call at certain frequency 
    // Basic functions
    float getTemperature(byte address);
    float getHeading();
    float getIrDistance(int pin, int samples);
    float getGray();
    float readings[SensorType_Count];
    float getRange();
    void ks109Init();
    Servo towerServo;
  private:
};


#endif
