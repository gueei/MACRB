#include <Event.h>
#include <Timer.h>

#include "Configuration.h"
#include <i2cmaster.h>

#include <Servo.h>
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
float initOrientation = 0;
boolean rotate = false;
void setup(){
  // Hardware bus init
  i2c_init();
  Serial.begin(115200);
  delay(500);
  
  Serial.println("START");
  drive.enableMotor(false);
  sensors.init();
  delay(100);
  
  initOrientation = sensors.getHeading();
  
  /*
  drive.enableMotor(true);
  
  turnToHeading(NORTH);
  moveForward(30);
  turnToHeading(WEST);
  moveForward(30);
  turnToHeading(SOUTH);
  moveForward(30);
  turnToHeading(EAST);
  moveForward(30);
  */
  
  drive.enableMotor(false);
  
  
}

void loop(){
  sensors.checkAllValues();
  delay(500);
}

void moveForward(float distanceCm){
  drive.forwardDistance(2000, distanceCm);
  while(drive.distanceToGo()!=0){
    drive.run();
  }
}

void turnToHeading(float targetHeading){
  // Tolerance 0.01
  float error;
  do{
    sensors.checkAllValues();
    error = sensors.getHeading() - targetHeading;
    if (error>PI) error = error - 2*PI;
    else if (error<-PI) error = error + 2*PI;
    drive.rotateAngle(500, error);
    Serial.println(error);
    while(drive.distanceToGo()!=0){
      drive.run();
    }
  }while(abs(error)>0.003);
}
