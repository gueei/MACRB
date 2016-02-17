#include "Arduino.h"
#include "Configuration.h"
#include "Sensors.h"
#include <Wire.h>
#include <Servo.h>

Sensors::Sensors(){}

void Sensors::init(){
  towerServo.attach(SENSOR_SERVO_PIN);
  towerServo.writeMicroseconds(SENSOR_SERVO_CENTER);
  
  // init Mangetometer
  Wire.beginTransmission(HMC5883L_Address);
  Serial.println("Init HMC5883L");
  Wire.write(0x01);
  Wire.write(Compass_Scale_Setting);
  Wire.endTransmission();

  // Mode continuous
  Wire.beginTransmission(HMC5883L_Address);
  Serial.println("Set Mode to Continuous");
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();
  
  // init Range
  Serial.println("KS109 Init");
  ks109Init();

  for(int i=0; i<SensorType_Count; i++){
    readings[i] = -1;
  }
}

void Sensors::ks109Init(){
  Wire.beginTransmission(KS109);
  Wire.write(0x02);
  Wire.write(0x71);
  Wire.endTransmission();
}

// Fill in the sensor values with the current readings
void Sensors::checkAllValues(){
  readings[Heading] = getHeading();
  readings[Temp_Left] = getTemperature(TEMP_LEFT_ADDR);
  readings[Temp_Right] = getTemperature(TEMP_RIGHT_ADDR);
  readings[Dist_Left] = getIrDistance(DIST_LEFT_PIN, 1);
  readings[Dist_Right] = getIrDistance(DIST_RIGHT_PIN, 1);
  readings[FloorGray] = getGray();
  
  Serial.println("Range");
  readings[Dist_Front] = getRange();
  
#if DEBUGLEVEL > 2
  Serial.println("DEBUGLEVEL 3\tSensors::checkAllValues()");
  for(int i=0; i<SensorType_Count; i++){
    Serial.print(readings[i]);
    Serial.print("\t"); 
  }
  Serial.println();
#endif
}

float Sensors::getIrDistance(int pin, int samples){
  double total = 0;
  for(int i=0; i<samples; i++){
    total += (1 / (0.000413153 * analogRead(pin) - 0.0055266887)) * 10;
  }
  total /= samples;
  if (total<37 || total>370) total = -1; // Out of range
  return (float)total;
}

float Sensors::getGray(){
  return analogRead(GRAY_PIN);
}

float Sensors::getHeading(){
  uint8_t buffer[6];
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(0x03);
  Wire.endTransmission(false);
  Wire.requestFrom(HMC5883L_Address, 6);
  Wire.write(0x03);
  for(int i=0; i<6; i++)
    buffer[i] = Wire.read();

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

float Sensors::getRange(){
  Wire.beginTransmission(KS109);
  Wire.write(0x02);
  Wire.write(0xb8);
  Wire.endTransmission();

  Wire.beginTransmission(KS109);
  Wire.write(0x02);
  Wire.endTransmission();
  
  delay(80);

  Wire.requestFrom(KS109, 2);
  int high = Wire.read();
  int low = Wire.read();
  
  return (high << 8) + low;
}

float Sensors::getTemperature(byte address) {
  return 0;
}
