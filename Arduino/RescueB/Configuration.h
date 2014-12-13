#include "Arduino.h"

// Stepper Motor definitions
#define LF_DIR 28
#define LF_STEP 26
#define LF_EN 24
#define LB_DIR A1
#define LB_STEP A0
#define LB_EN 30
#define RF_DIR 34
#define RF_STEP 36
#define RF_EN 38
#define RB_DIR A7
#define RB_STEP A6
#define RB_EN A2

#define STEPS_FOR_1CM  80.7
#define STEPS_FOR_360 5400
#define ACCELERATION 500
