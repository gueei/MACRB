#include "Arduino.h"

// Stepper Motor definitions
#define LF_DIR 28
#define LF_STEP 26
#define LF_EN 24
#define LB_DIR 34
#define LB_STEP 36
#define LB_EN 30
#define RF_DIR A1
#define RF_STEP A0
#define RF_EN 38
#define RB_DIR A7
#define RB_STEP A6
#define RB_EN A2

#define STEPS_FOR_1CM  80.7
#define STEPS_FOR_360 6415
#define ACCELERATION 500

// Software I2C
// MLX90614
#define TEMP_LEFT_ADDR 0x54<<1
#define TEMP_RIGHT_ADDR 0x55<<1

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

#define Compass_Scale  0.92
#define Compass_Scale_Setting  0x01 << 5

// Ir Distance
#define DIST_LEFT_PIN  A9
#define DIST_RIGHT_PIN  A10

// Ultrasonic front - TODO
#define DIST_FRONT_ADDR  0x01<<1

// Timer event trigger millisecond
#define TIMER_SENSOR  200
#define TIMER_DECISION  200

