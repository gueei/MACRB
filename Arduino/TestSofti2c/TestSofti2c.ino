#include <Wire.h>

//#include <SoftI2CMaster.h>

#define HMC5883L_Address 0x1E
#define ConfigurationRegisterA 0x00
#define ConfigurationRegisterB 0x01
#define ModeRegister 0x02
#define DataRegisterBegin 0x03

#define Measurement_Continuous 0x00
#define Measurement_SingleShot 0x01
#define Measurement_Idle 0x03

//SoftI2CMaster Wire = SoftI2CMaster();

struct MagnetometerRaw
{
	int XAxis;
	int YAxis;
	int ZAxis;
};

void setup(){
  //Wire.setPins(21, 20, true);
  Wire.begin();
  setMeasurementMode(Measurement_Continuous);
  Serial.begin(115200);
}

void loop(){
  uint8_t* buffer = hRead(DataRegisterBegin, 6);
  MagnetometerRaw raw = MagnetometerRaw();
  raw.XAxis = (buffer[0] << 8) | buffer[1];
  raw.ZAxis = (buffer[2] << 8) | buffer[3];
  raw.YAxis = (buffer[4] << 8) | buffer[5];
  Serial.println(raw.XAxis);
  delay(500);
}

void hWrite(int address, int data)
{
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t* hRead(int address, int length)
{
  Wire.beginTransmission(HMC5883L_Address);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.beginTransmission(HMC5883L_Address);
  Wire.requestFrom(HMC5883L_Address,length);

  uint8_t buffer[length];

	  for(uint8_t i = 0; i < length; i++)
	  {
		  buffer[i] = Wire.read();
	  }

  Wire.endTransmission();

  return buffer;
}

int setMeasurementMode(uint8_t mode)
{
  hWrite(ModeRegister, mode);
}
