#include <Event.h>
#include <Timer.h>

#include "Configuration.h"
#include <Wire.h>
#include <HMC5883L.h>

#include <AccelStepper.h>
#include "Drivebase.h"
#include "Sensors.h"

#include "RescueBTask.h"

// Timer for regular executing relevant functions
// e.g. take sensor values, print the state to serial etc. 
Timer timer;
int sensorRecordEvent, decisionEvent;

RescueBTask task = RescueBTask();

//Drivebase drive;
Sensors sensors;
int mode = 1;
void setup(){
  Serial.begin(115200);
  Serial.println("START");
  
  Wire.begin();
  sensors.init();
  //drive.enableMotor(false);
  // i2c_init();
  
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
