#include "Configuration.h"
#include "Drivebase.h"
#include "Sensors.h"

#ifndef RescueBTask_h
#define RescueBTask_h

// Define more state if necessary

class RescueBTask{
  public:
    enum State{
        Stopped,
        Rotating,
        Moving,
        Victim_signaling
    };
    void onDecision(Drivebase& drive, Sensors& sensor);
    void onLogging();
    RescueBTask();
    State state;
    
  private:
    void doStop(Drivebase& drive, Sensors& sensor);
    void doRotating(Drivebase& drive, Sensors& sensor);
    void doMoving(Drivebase& drive, Sensors& sensor);
    void doVictim_Signaling(Drivebase& drive, Sensors& sensor);
};

#endif
