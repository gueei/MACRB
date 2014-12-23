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
// MLX90614 - Channel 1
#define TEMP1_ADDR 0xAA<<1

// HMC5883L - Compass
#define HMC5883L_Address 0x1E
#define ConfigurationRegisterA 0x00
#define ConfigurationRegisterB 0x01
#define ModeRegister 0x02
#define DataRegisterBegin 0x03

#define Measurement_Continuous 0x00
#define Measurement_SingleShot 0x01
#define Measurement_Idle 0x03

// Timer event trigger millisecond
#define TIMER_SENSOR  200
#define TIMER_DECISION  200

