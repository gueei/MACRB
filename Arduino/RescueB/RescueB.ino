#define DEBUGLEVEL  4 // 0 - no debug, 1 - basic, 5 - verbal
//#define DEBUGSENSOR 0

#include <Event.h>
#include <Timer.h>

#include "Configuration.h"
#include <i2cmaster.h>

#include <AccelStepper.h>
#include "Drivebase.h"
#include "Sensors.h"

#include "RescueBTask.h"

// Timer for regular executing relevant functions
// e.g. take sensor values, print the state to serial etc. 
Timer timer;
int sensorRecordEvent, decisionEvent;

RescueBTask task = RescueBTask();

Drivebase drive;
Sensors sensors;
int mode = 1;
void setup(){
  // Hardware bus init
  i2c_init();
  Serial.begin(115200);
  delay(500);
  
  

  Serial.println("START");
  drive.enableMotor(false);
  sensors.init();
  
  decisionEvent = timer.every(TIMER_DECISION, makeDecision);
//  drive.enableMotor(true);
}

void loop(){
  // Motor Driver need to run fastest possible
  //drive.run();
  // Timer need to update fastest possible, too 
  timer.update();
  // Serial Event for handling Debug Mode
  // TODO
  Serial.println(sensors.getHeading());
  
  delay(500);
}

// This replaced the main loop
void makeDecision(){
  // Get Sensor Values
  
  // Call the decision Maker class
  //task.onDecision(drive, sensors);
  // Log the sensor values, if necessary
  task.onLogging();
}
