#include "Arduino.h"
#include <AccelStepper.h>
AccelStepper stepper(1, A0, A1);

void setup()
{  
   Serial.begin(115200);           // set up Serial library at 9600 bps
   Serial.println("Stepper test!");
  
  pinMode(38, OUTPUT);
  pinMode(A2, OUTPUT);
   digitalWrite(38, LOW);
   digitalWrite(A2, LOW);
   stepper.moveTo(-2000);
   stepper.setMaxSpeed(1000);	
   stepper.setSpeed(1000);
   //stepper.setCurrentPosition(-5000);
}

void loop()
{
  if(!stepper.runSpeedToPosition()){
    delay(2000);
    stepper.moveTo(2000);
  }
  //stepper.runSpeed();
}

