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
  Serial.println(F("Init HMC5883L"));
  Wire.write(0x01);
  Wire.write(Compass_Scale_Setting);
  Wire.endTransmission();

  // Mode continuous
  Wire.beginTransmission(HMC5883L_Address);
  Serial.println(F("Set Mode to Continuous"));
  Wire.write(0x02);
  Wire.write(0x00);
  Wire.endTransmission();
  
  // init Range
  Serial.println(F("KS109 Init"));
  ks109Init();
}

void Sensors::ks109Init(){
  Wire.beginTransmission(KS109);
  Wire.write(0x02);
  Wire.write(0x71);
  Wire.endTransmission();
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
  int dev = address;
  int data_low = 0;
  int data_high = 0;
  int pec = 0;
  Wire.beginTransmission(address);
  Wire.write(0x07);
  Wire.endTransmission(false);

  Wire.requestFrom(address, 3);
  data_low = Wire.read();
  data_high = Wire.read();
  pec= Wire.read();

  // This converts high and low bytes together and processes temperature, 
  // MSB is a error bit and is ignored for temps.
  double tempFactor = 0.02;       // 0.02 degrees per LSB (measurement 
                                  // resolution of the MLX90614).
  double tempData = 0x0000;       // Zero out the data
  int frac;                       // Data past the decimal point

  // This masks off the error bit of the high byte, then moves it left 
  // 8 bits and adds the low byte.
  tempData = (double)(((data_high & 0x007F) << 8) + data_low);
  tempData = (tempData * tempFactor)-0.01;
  float celcius = tempData - 273.15;
  
  // Returns temperature un Celcius.
  return celcius;
}

float Sensors::getTemperatureLeft(){
  return getTemperature(TEMP_LEFT_ADDR);
}

float Sensors::getTemperatureRight(){
  return getTemperature(TEMP_RIGHT_ADDR);
}


