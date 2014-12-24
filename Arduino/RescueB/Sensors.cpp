#include "Arduino.h"
#include "Configuration.h"
#include "Sensors.h"
#include <i2cmaster.h>

Sensors::Sensors(){}

void Sensors::init(){
  // init Mangetometer
  // 0.73 Resolution
  i2c_start_wait(HMC5883L_Address_Write);
  Serial.println(i2c_write(0x01));
  Serial.println(i2c_write(Compass_Scale_Setting));
  i2c_stop();

  // Mode continuous
  i2c_start_wait(HMC5883L_Address_Write);
  Serial.println(i2c_write(0x02));
  Serial.println(i2c_write(0x00));
  i2c_stop();
  
#if DEBUGLEVEL > 2
  i2c_start_wait(HMC5883L_Address_Write);
  i2c_write(0x01);
  i2c_rep_start(HMC5883L_Address_Read);
  Serial.print("Scale setting: ");
  Serial.println(i2c_readNak());
  i2c_stop();
#endif

  for(int i=0; i<SensorType_Count; i++){
    readings[i] = -1;
  }
}

// Fill in the sensor values with the current readings
void Sensors::checkAllValues(){
  readings[Heading] = getHeading();
  readings[Temp_Left] = getTemperature(TEMP_LEFT_ADDR);
  readings[Temp_Right] = getTemperature(TEMP_RIGHT_ADDR);
  
#if DEBUGLEVEL > 3
  Serial.println("DEBUGLEVEL 3\tSensors::checkAllValues()");
  for(int i=0; i<SensorType_Count; i++){
    Serial.print(readings[i]);
    Serial.print("\t"); 
  }
  Serial.println();
#endif
}

float Sensors::getTemperature(byte address) {
  return -1;
}

float Sensors::getHeading(){
  uint8_t buffer[6];
  i2c_start_wait(HMC5883L_Address_Write);
  i2c_write(0x03);
  i2c_rep_start(HMC5883L_Address_Read);
  buffer[0] = i2c_readAck();
  buffer[1] = i2c_readAck();
  buffer[2] = i2c_readAck();
  buffer[3] = i2c_readAck();
  buffer[4] = i2c_readAck();
  buffer[5] = i2c_readNak();
  i2c_stop();

  float xaxis = ((buffer[0] << 8) | buffer[1]) * Compass_Scale;
  float zaxis = ((buffer[2] << 8) | buffer[3]) * Compass_Scale;
  float yaxis = ((buffer[4] << 8) | buffer[5]) * Compass_Scale;

#if DEBUGLEVEL > 4
  Serial.print(xaxis);
  Serial.print("\t");
  Serial.print(yaxis);
  Serial.print("\t");
  Serial.print(zaxis);
  Serial.print("\t");
  Serial.println();
#endif

  float heading = atan2(yaxis, xaxis) + 0.0457;
  
  
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  
  return heading;
}
