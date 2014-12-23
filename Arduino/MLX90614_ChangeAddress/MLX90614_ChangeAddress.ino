#define SDA_PORT PORTC
#define SDA_PIN 4
#define SCL_PORT PORTC
#define SCL_PIN 5
#define I2C_TIMEOUT 100
#define I2C_NOINTERRUPT 0
#define I2C_SLOWMODE 1
#define FAC 1
#define I2C_CPUFREQ (F_CPU/FAC)

#include <SoftI2CMaster.h>

void setup(){
 Serial.begin(115200);
 Serial.println("Setup...");
 
 i2c_init();                              //Initialise the i2c bus
 
 delay(5000);                    // Wait to allow serial connection
 ReadAddr(0);                    // Read current address bytes
 //ChangeAddr(0x55, 0x00);         // Change address to new value
 //ChangeAddr(0x5A, 0xBE);       // Change address to default value
 //ChangeAddr2(0x5A);
 ReadAddr(0);                    // Read address bytes
 delay(5000);                    // Cycle power to MLX during this pause
 ReadTemp(0);                    // Read temperature using default address
 ReadTemp(0x55<<1);              // Read temperature using new address
}

void loop(){
   delay(1000); // wait a second
}

word ChangeAddr2(byte NewAddr){
  i2c_start_wait(0 + I2C_WRITE); 
  i2c_write(0x2E);
  i2c_write(0x00);
  i2c_write(0x00);
  i2c_write(0x6F);
  delay(1000);
  i2c_start_wait(0+I2C_WRITE);  //send start condition and write bit 
  i2c_write(0x2E);  //send command for device to return address
  i2c_write(0x50);  // send low byte of address
  i2c_write(0x00);  //send high byte of address 
  i2c_write(0x63); // send PEC 
  i2c_stop(); //Release bus, end transaction 
}

word ChangeAddr(byte NewAddr1, byte NewAddr2) {
 Serial.println("> Change address");

 i2c_start_wait(0 + I2C_WRITE);    //send start condition and write bit
 Serial.print("Write command");
 Serial.println(i2c_write(0x2E));                  //send command for device to return address
 Serial.print("Low Byte");
 Serial.println(i2c_write(0x00));                  // send low byte zero to erase
 Serial.println("HIGH Byte");
 Serial.println(i2c_write(0x00));
 //i2c_write(0x00);                  //send high byte zero to erase
 if (i2c_write(0x6F) == 0) {
   i2c_stop();                     //Release bus, end transaction
   Serial.println("  Data erased.");
 }
 else {
   i2c_stop();                     //Release bus, end transaction
   Serial.println("  Failed to erase data");
   //return -1;
 }

 delay(200);
 
 Serial.print("  Writing data: ");
 Serial.print(NewAddr1, HEX);
 Serial.print(", ");
 Serial.println(NewAddr2, HEX);

 for (int a = 0; a != 256; a++) {
   i2c_start_wait(0 + I2C_WRITE);  //send start condition and write bit
   i2c_write(0x2E);                //send command for device to return address
   i2c_write(NewAddr1);            // send low byte zero to erase
   i2c_write(NewAddr2);            //send high byte zero to erase
   if (i2c_write(a) == 0) {
     i2c_stop();                   //Release bus, end transaction
     delay(100);                   // then wait 10ms
     Serial.print("Found correct CRC: 0x");
     Serial.println(a, HEX);
     return a;
   }
 }
 i2c_stop();                       //Release bus, end transaction
 Serial.println("Correct CRC not found");
 return -1;
}

void ReadAddr(byte Address) {

 Serial.println("> Read address");

 Serial.print("  MLX address: ");
 Serial.print(Address, HEX);
 Serial.print(", Data: ");

 i2c_start_wait(Address + I2C_WRITE);  //send start condition and write bit
 i2c_write(0x2E);                  //send command for device to return address
 i2c_rep_start(Address + I2C_READ);
 
 Serial.print(i2c_read(false), HEX); //Read 1 byte and then send ack
 Serial.print(", ");
 Serial.print(i2c_read(false), HEX); //Read 1 byte and then send ack
 Serial.print(", ");
 Serial.println(i2c_read(true), HEX);
 i2c_stop();
}

float ReadTemp(byte Address) {
 int data_low = 0;
 int data_high = 0;
 int pec = 0;

 Serial.println("> Read temperature");

 Serial.print("  MLX address: ");
 Serial.print(Address, HEX);
 Serial.print(", ");

 i2c_start_wait(Address + I2C_WRITE);
 i2c_write(0x07);                  // Address of temp bytes
 
 // read
 i2c_rep_start(Address + I2C_READ);
 data_low = i2c_read(false);         //Read 1 byte and then send ack
 data_high = i2c_read(false);        //Read 1 byte and then send ack
 pec = i2c_read(true);
 i2c_stop();
 
 //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
 float Temperature = 0x0000;       // zero out the data
 
 // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
 Temperature = (float)(((data_high & 0x007F) << 8) + data_low);
 Temperature = (Temperature * 0.02) - 273.16;
 
 Serial.print(Temperature);
 Serial.println(" C");
 return Temperature;
}
