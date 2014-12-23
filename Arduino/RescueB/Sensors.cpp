#include "Arduino.h"
#include "Configuration.h"
#include "Sensors.h"
#include <Wire.h>
#include <HMC5883L.h>

Sensors::Sensors(){}

void Sensors::init(){
  compass = HMC5883L();
  compass.SetScale(1.3);
  compass.SetMeasurementMode(Measurement_Continuous);
}

float Sensors::getTemperature(byte address) {
  return -1;
}

float Sensors::getHeading(){
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  float declinationAngle = 0.0457;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 
  
  return headingDegrees;
}
