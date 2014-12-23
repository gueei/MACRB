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
  }
}

void RescueBTask::doStop(Drivebase& drive, Sensors& sensor){
  // Stop means do nothing, really :P
}

void RescueBTask::onLogging(){}
