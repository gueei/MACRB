#include "Arduino.h"
#include "Configuration.h"
#include <AccelStepper.h>
#include "Drivebase.h"

Drivebase::Drivebase(){
  pinMode(LF_EN, OUTPUT);
  pinMode(RF_EN, OUTPUT);
  pinMode(LB_EN, OUTPUT);
  pinMode(RB_EN, OUTPUT);
  lf=AccelStepper(1, LF_STEP, LF_DIR);
  rf=AccelStepper(1, RF_STEP, RF_DIR);
  lb=AccelStepper(1, LB_STEP, LB_DIR);
  rb=AccelStepper(1, RB_STEP, RB_DIR);
  
  lf.setAcceleration(ACCELERATION);
  lb.setAcceleration(ACCELERATION);
  rf.setAcceleration(ACCELERATION);
  rb.setAcceleration(ACCELERATION);
}

void Drivebase::enableMotor(bool enable){
  digitalWrite(LF_EN, !enable);
  digitalWrite(RF_EN, !enable);
  digitalWrite(LB_EN, !enable);
  digitalWrite(RB_EN, !enable);
}

void Drivebase::run(){
  lf.run();
  rf.run();
  lb.run();
  rb.run();
}

void Drivebase::forwardSteps(int spd, long steps){
  lf.setMaxSpeed(spd);
  lb.setMaxSpeed(spd);
  rf.setMaxSpeed(spd);
  rb.setMaxSpeed(spd);
  lf.setCurrentPosition(0);
  rf.setCurrentPosition(0);
  lb.setCurrentPosition(0);
  rb.setCurrentPosition(0);
  lf.moveTo(steps);
  rf.moveTo(steps);
  rb.moveTo(steps);
  lb.moveTo(steps);  
}

long Drivebase::distanceToGo(){
  return lf.distanceToGo();
}

void Drivebase::forwardDistance(int spd, float distanceCM){
  long steps = distanceCM * STEPS_FOR_1CM;
  forwardSteps(spd, steps);
}

void Drivebase::rotateSteps(int spd, long steps){
  lf.setMaxSpeed(spd);
  lb.setMaxSpeed(spd);
  rf.setMaxSpeed(spd);
  rb.setMaxSpeed(spd);
  lf.setCurrentPosition(0);
  rf.setCurrentPosition(0);
  lb.setCurrentPosition(0);
  rb.setCurrentPosition(0);
  lf.moveTo(steps);
  lb.moveTo(steps);
  rf.moveTo(-steps);
  rb.moveTo(-steps);
}

void Drivebase::rotateAngle(int spd, float angle){
  long steps = angle / (2*PI) * (float)STEPS_FOR_360;
  rotateSteps(spd, steps);
}


