#include "Arduino.h"
#include "Configuration.h"

#ifndef Drivebase_h
#define Drivebase_h

#include <AccelStepper.h>

class Drivebase{
public:
  Drivebase();
  long distanceToGo();
  void enableMotor(bool enable);
  
  void run();
  
  // For constant speed use
  void runSpeed();
  void setCurrentPosition(long pos);
  long currentPosition();
  void setSpeed(int spd);
  void moveTo(long steps);
  void setMaxSpeed(int spd);
  
  void forwardSteps(int spd, long steps);
  void forwardDistance(int spd, float distanceCM);
  void rotateSteps(int spd, long steps);
  void rotateAngle(int spd, float angle);
private:
  AccelStepper lf, lb, rf, rb;
};
#endif
