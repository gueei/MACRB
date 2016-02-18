#define DEBUGLEVEL  0 // 0 - no debug, 1 - basic, 5 - verbal
#include "Arduino.h"

#define NORTH 4.65
#define SOUTH 1.67
#define EAST 6.04
#define WEST 3.29

#define MAP_MAX_WIDTH 10
#define MAP_MAX_HEIGHT 6

#define  MAP_WIDTH    5
#define  MAP_HEIGHT   3

#define ENTRANCEX  3
#define ENTRANCEY  1

#define BLACK_THRESHOLD 50
#define VICTIM_TEMP  28

// Motor pin definitions
#define EN1 4

// Software I2C
// MLX90614
#define TEMP_LEFT_ADDR 0x5C
#define TEMP_RIGHT_ADDR 0x55

// HMC5883L - Compass
#define HMC5883L_Address 0x1E
#define HMC5883L_Address_Write 0x3C
#define HMC5883L_Address_Read 0x3D

#define ConfigurationRegisterA 0x00
#define ConfigurationRegisterB 0x01
#define ModeRegister 0x02
#define DataRegisterBegin 0x03

#define Measurement_Continuous 0x00
#define Measurement_SingleShot 0x01
#define Measurement_Idle 0x03

#define Compass_Scale  0.73
#define Compass_Scale_Setting  0x00 << 5

// Ir Distance
#define DIST_LEFT_PIN  1
#define DIST_RIGHT_PIN  2

// Ground Gray Sensor
#define GRAY_PIN  3

// Ultrasonic front - TODO
#define KS109  0x74

// Servo for sensor tower
#define SENSOR_SERVO_PIN 11
#define SENSOR_SERVO_CENTER 1586 // microsecond

// LED
#define LED_PIN  13

// Calibration Mode
#define CALIBRATION_MODE_PIN  40

// Timer event trigger millisecond
#define TIMER_SENSOR  200
#define TIMER_DECISION  1000


