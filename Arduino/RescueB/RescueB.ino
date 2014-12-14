#include "Configuration.h"
#include <AccelStepper.h>
#include "Drivebase.h"

Drivebase drive;
void setup(){
  drive.enableMotor(true);
  drive.rotateAngle(1000, 2*PI);
}

void loop(){
  drive.run();
  if(drive.distanceToGo()==0){
    //drive.rotateAngle(500, PI/2);
    
    drive.enableMotor(false);
  }
}
