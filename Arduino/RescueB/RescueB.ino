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
  
  Map rmap = Map(1,2);
  rmap.debugMap(1, 2);
  
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
      rmap.addVisit(current);
      float values;
      sensors.checkAllValues();
      
      if(sensors.readings[Sensors::Dist_Left]<=2000){
        //rmap.setWall(current.x, current.y, wallDir(dir, West));
      }
      else if(sensors.readings[Sensors::Dist_Right]<=2000){
        //rmap.setWall(current.x, current.y, wallDir(dir, East));
      }
      else if(sensors.readings[Sensors::Dist_Front]<=200){
        //rmap.setWall(current.x, current.y, wallDir(dir, North));
      }
        
      StackArray <Coordinate> stack;
      stack=rmap.findPath(current, dir);
      

      rmap.printMap();


      Serial.println("Current Tile");
      rmap.printCoordinate(current);
      Serial.println();
      Serial.println("Current Dir");
      Serial.println(dir);

      rmap.printCoordinate(stack.pop());
      
      Coordinate nextTile =stack.pop();

      
      Serial.println("Next Tile");
      rmap.printCoordinate(nextTile);
      
      delay(5000);
      
      int cx = current.x, cy = current.y, nx = nextTile.x, ny = nextTile.y;
      
      if(cx==nx && cy==ny-1) {
      //  turnToHeading(NORTH);
        //forwardToNextTile();
        dir = North;
      }else if (cx==nx-1 && cy==ny){
        //turnToHeading(WEST);
        //forwardToNextTile();
        dir = West;
      }else if (cx==nx && cy==ny+1){
        //turnToHeading(EAST);
        //forwardToNextTile();
        dir = East;
      }else{
        //turnToHeading(SOUTH);
        //forwardToNextTile();
        dir = South;
      }
      
      current.x=nextTile.x;
      current.y=nextTile.y;
        
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
    error = range- (tiles_Infront*300) - 100;
    moveForward(error * 0.95 /10);
  } while(abs(error)>3.5);
}
