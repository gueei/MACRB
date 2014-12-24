#include "Configuration.h"
#include "Drivebase.h"
#include "Sensors.h"
#include "RescueBTask.h"

RescueBTask::RescueBTask(){
  state = Stopped;
}

void RescueBTask::onDecision(Drivebase& drive, Sensors& sensor){
  switch(state){
    case Stopped:
      doStop(drive, sensor);
    break;
    case Moving:
      doMoving(drive, sensor);
    break;
  }
}

void RescueBTask::doStop(Drivebase& drive, Sensors& sensor){
  // Stop means do nothing, really :P
}

void RescueBTask::doMoving(Drivebase& drive, Sensors& sensor){
  // Check whether the desire distance reached or not
  // using drive.distanceToGo
  
  
}

void RescueBTask::onLogging(){}
