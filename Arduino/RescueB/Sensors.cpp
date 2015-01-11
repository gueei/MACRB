#include "Arduino.h"
#include "Configuration.h"
#include "Sensors.h"
#include <i2cmaster.h>
#include <Servo.h>

Sensors::Sensors(){}

void Sensors::init(){
  towerServo.attach(SENSOR_SERVO_PIN);
  towerServo.writeMicroseconds(SENSOR_SERVO_CENTER);
  
  // init Mangetometer
  i2c_start_wait(HMC5883L_Address_Write);
  Serial.println(i2c_write(0x01));
  Serial.println(i2c_write(Compass_Scale_Setting));
  i2c_stop();

  // Mode continuous
  i2c_start_wait(HMC5883L_Address_Write);
  Serial.println(i2c_write(0x02));
  Serial.println(i2c_write(0x00));
  i2c_stop();
  
#if DEBUGLEVEL > 4
  i2c_start_wait(HMC5883L_Address_Write);
  i2c_write(0x01);
  i2c_rep_start(HMC5883L_Address_Read);
  Serial.print("Scale setting: ");
  Serial.println(i2c_readNak());
  i2c_stop();
#endif

  // init Range
#if DEBUGLEVEL > 4
  Serial.println("KS109 Init");
#endif
  ks109Init();

  for(int i=0; i<SensorType_Count; i++){
    readings[i] = -1;
  }
}

void Sensors::ks109Init(){
  i2c_start_wait(KS109);             // Start communticating with KS103
  i2c_write(0x02);                              // Send Reg
  i2c_write(0x71);                                // Send 0x72 to set USB Power
  i2c_stop();
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
    total += (1 / (0.000413153 * analogRead(pin) - 0.0055266887));
  }
  total /= samples;
  if (total<3.7 || total>37) total = -1; // Out of range
  return (float)total;
}

float Sensors::getGray(){
  return analogRead(GRAY_PIN);
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

float Sensors::getRange(){
  i2c_start_wait(KS109);             // Start communticating with KS103
  i2c_write(0x02);                              // Send Reg
  i2c_write(0xb8);                                // Send 0x72 to set USB Power
  i2c_stop();
  
  i2c_start_wait(KS109);             // Start communticating with KS103
  i2c_write(0x02);
  i2c_stop();
  
  delay(80);
  
  i2c_start_wait(KS109+1);             // Start communticating with KS103
  
  int high = i2c_readAck();
  int low = i2c_readNak();
  
  int range = (high << 8) + low;
  
  i2c_stop();
  return range;
}

float Sensors::getTemperature(byte address) {
  int dev = address;
  int data_low = 0;
  int data_high = 0;
  int pec = 0;

  // Write
  i2c_start_wait(dev+I2C_WRITE);
  i2c_write(0x07);

  // Read
  i2c_rep_start(dev+I2C_READ);
  data_low = i2c_readAck();       // Read 1 byte and then send ack.
  data_high = i2c_readAck();      // Read 1 byte and then send ack.
  pec = i2c_readNak();
  i2c_stop();

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
