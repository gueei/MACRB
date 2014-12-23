#include <i2cmaster.h>

byte newAddress = 0x55;      // Write here the new address you want
                                // to assign. In this example, the new
                                // address will be 0x55.
                                // Shift the address 1 bit right, the 
                                // I²Cmaster library only needs the 7 most 
                                // significant bits for the address.

void setup()
{
  Serial.begin(115200);                      // Start serial communication 
                                           // at 9600bps.

  i2c_init();                              // Initialise the i2c bus.
  PORTC = (1 << PORTC4) | (1 << PORTC5);   // Enable pullups.

  delay(5000);                             // Wait to allow serial connection.
  ChangeAddress(newAddress, 0x00);         // Changes address to new value.
}

void loop()
{
}

word ChangeAddress(byte NewAddr1, byte NewAddr2) {
  Serial.println("> Change address");

  i2c_start_wait(0 + I2C_WRITE);      // Send start condition and write bit.
  i2c_write(0x2E);                    // Send command for device to return 
                                      // address (0x2E).
  i2c_write(0x00);                    // Send low byte zero to erase.
  i2c_write(0x00);                    // Send high byte zero to erase.
  if (i2c_write(0x6F) == 0) {
    i2c_stop();                       // Release bus, end transaction.
    Serial.println("  Data erased."); // Address erase confirmation message.
  }
  else {
    i2c_stop();                       // Release bus, end transaction.
    Serial.println("  Failed to erase data");
    return -1;
  }

  Serial.print("  Writing data: ");
  Serial.print(NewAddr1, HEX);
  Serial.print(", ");
  Serial.println(NewAddr2, HEX);

  for (int a = 0; a != 256; a++) {
    i2c_start_wait(0 + I2C_WRITE);    // Send start condition and write bit.
    i2c_write(0x2E);                  // Send command for device to return 
                                      // address (0x2E).
    i2c_write(NewAddr1);              // Send low byte zero to assign new address.
    i2c_write(NewAddr2);              // Send high byte zero to assign new address.
    if (i2c_write(a) == 0) {
      i2c_stop();                     // Release bus, end transaction.
      delay(100);                     // Wait 10ms.
      Serial.print("Found correct CRC: 0x");
      Serial.println(a, HEX);
      return a;
    }
  }
  i2c_stop();                         // Release bus, end transaction.
  Serial.println("Correct CRC not found");
  return -1;
}
