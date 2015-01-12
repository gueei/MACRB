#include <Event.h>
#include <Timer.h>
#include <QueueList.h>
#include <StackArray.h>

#include "Configuration.h"
#include <i2cmaster.h>

#include <Servo.h>
#include <AccelStepper.h>
#include "Drivebase.h"
#include "Sensors.h"
#include "Map.h"

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
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  delay(100);
  
  //Map rmap = Map(MAP_WIDTH,MAP_HEIGHT);
  //Map::debugMap(0,2);
  
  if(digitalRead(CALIBRATION_MODE_PIN)){
    for(;;){
      sensors.checkAllValues();
      delay(500);
    }
  }

  taskRescue();
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
  }while(abs(error)>0.007);
}

void taskRescue(){
    drive.enableMotor(true);
    Map rmap = Map(ENTRANCEX, ENTRANCEY);
    
    Coordinate current;
    current.x = ENTRANCEX;
    current.y = ENTRANCEY;
    Direction dir = North;
    
    while( true ){
      sensors.checkAllValues();
      
      if(sensors.readings[Sensors::Dist_Left]<=200 && sensors.readings[Sensors::Dist_Left]>0){
        rmap.setWall(current.x, current.y, wallDir(dir, West));
      }
      if(sensors.readings[Sensors::Dist_Right]<=200 && sensors.readings[Sensors::Dist_Right]>0){
        rmap.setWall(current.x, current.y, wallDir(dir, East));
      }
      if(sensors.readings[Sensors::Dist_Front]<=200 && sensors.readings[Sensors::Dist_Front]>0){
        rmap.setWall(current.x, current.y, wallDir(dir, North));
      }
      
      rmap.addVisit(current);
      
      Coordinate entrance;
      entrance.x = ENTRANCEX;
      entrance.y = ENTRANCEY;
      StackArray<Coordinate> pathStack = rmap.findPath(current, dir, entrance);
      Serial.println(pathStack.count());
      
      if (pathStack.count()<=1) break;
      
      pathStack.pop(); // the starting
      Coordinate next = pathStack.pop();
      
      Serial.print("New destination");
      rmap.printCoordinate(next);
      Serial.println();

      rmap.printMap();
      
      
      float values;
      
      
      if (current.x == next.x){
        if (current.y > next.y) dir = North;
        else dir = South;
      }else{
        if (current.x>next.x) dir = West;
        else dir = East;
      }
      
      current = next;
      switch(dir){
        case North: turnToHeading(NORTH); break;
        case South: turnToHeading(SOUTH); break;
        case East: turnToHeading(EAST); break;
        default: turnToHeading(WEST);
      }
      
      forwardToNextTile();
      
      //delay(7000);
    }
    
    drive.enableMotor(false);
}

Direction wallDir(Direction current, Direction relative){
  if (current==North){
    return relative;
  }
  if (current==South){
    if (relative==North) return South;
    if (relative==East) return West;
    if (relative==West) return East;
    else return North;
  }
  if (current==East){
    if (relative==North) return East;
    if (relative==East) return South;
    if (relative==West) return North;
    else return West;
  }
  if (current==West){
    if (relative==North) return West;
    if (relative==East) return North;
    if (relative==West) return South;
    else return East;
  }
}

void forwardToNextTile(){
  moveForward (25);
  float error = 5;
  do {
    sensors.checkAllValues();
    float range = sensors.readings[Sensors::Dist_Front];
    int tiles_Infront = floor(range/300);
    error = range- (tiles_Infront*300) - 90;
    moveForward(error * 0.95 /10);
  } while(abs(error)>5);
}
