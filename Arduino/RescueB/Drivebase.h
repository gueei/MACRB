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
  
  void forwardSteps(int spd, long steps);
  void forwardDistance(int spd, float distanceCM);
  void rotateSteps(int spd, long steps);
  void rotateAngle(int spd, float angle);
private:
  AccelStepper lf, lb, rf, rb;
};
#endif
