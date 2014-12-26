#include <i2cmaster.h>

#define HMC5883L_Address 0x1E
#define ConfigurationRegisterA 0x00
#define ConfigurationRegisterB 0x01
#define ModeRegister 0x02
#define DataRegisterBegin 0x03

#define Measurement_Continuous 0x00
#define Measurement_SingleShot 0x01
#define Measurement_Idle 0x03

void setup(){
  Serial.begin(115200);
  Serial.println("START");

  i2c_init();
  delay(100);
  
  ks109Init();
  delay(2000);
  
  magnetometerInit();
  delay(1000);
  
  
}

void loop(){
  Serial.print(180 * heading() / PI);
  Serial.print("\t");
  Serial.println();
  getRange();
  //Serial.println(temperatureCelcius(0x55<<1));
  delay(1000);
}

void ks109Init(){
  Serial.println("init ks109");
  i2c_start_wait(0xe8);             // Start communticating with KS103
  i2c_write(0x02);                              // Send Reg
  i2c_write(0x71);                                // Send 0x72 to set USB Power
  i2c_stop();
}

void getRange(){
  i2c_start_wait(0xe8);             // Start communticating with KS103
  i2c_write(0x02);                              // Send Reg
  i2c_write(0xbc);                                // Send 0x72 to set USB Power
  i2c_stop();
  
  delay(50);
  i2c_start_wait(0xe8);             // Start communticating with KS103
  i2c_write(0x02);
  i2c_stop();
  
  delay(50);
  
  i2c_start_wait(0xe9);             // Start communticating with KS103
  
  int high = i2c_readAck();
  int low = i2c_readNak();
  
  int range = (high << 8) + low;
  
  i2c_stop();
  
  Serial.println("RANGE");
  Serial.print(high, HEX);
  Serial.print("\t");
  Serial.print(low, HEX);
  Serial.print("\t");
  Serial.println(range, DEC);
}

void magnetometerInit(){
  Serial.println("init magnetometer");
  i2c_start_wait(0x3C);
  Serial.println(i2c_write(0x01));
  Serial.println(i2c_write(0x01 << 5));
  i2c_stop();

// Mode continuous
  i2c_start_wait(0x3C);
  Serial.println(i2c_write(0x02));
  Serial.println(i2c_write(0x00));
  i2c_stop();
}

float heading(){
  uint8_t buffer[6];
  i2c_start_wait(0x3C);
  i2c_write(0x03);
  i2c_rep_start(0x3D);
  buffer[0] = i2c_readAck();
  buffer[1] = i2c_readAck();
  buffer[2] = i2c_readAck();
  buffer[3] = i2c_readAck();
  buffer[4] = i2c_readAck();
  buffer[5] = i2c_readNak();
  i2c_stop();

  int xaxis = ((buffer[0] << 8) | buffer[1]) * 0.92;
  int yaxis = ((buffer[2] << 8) | buffer[3]) * 0.92;
  int zaxis = ((buffer[4] << 8) | buffer[5]) * 0.92;
  
#ifndef DEBUG
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

float temperatureCelcius(int address) {
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
