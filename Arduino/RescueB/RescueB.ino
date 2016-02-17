#include <QueueList.h>
#include <StackArray.h>

#include "Configuration.h"

#include <Servo.h>
#include "Sensors.h"
#include "Map.h"
#include <Wire.h>
#include <MemoryFree.h>

Sensors sensors;
int mode = 1;
float initOrientation = 0;
boolean rotate = false;
void setup(){
  // Hardware bus init
  
  Serial.begin(9600);
  delay(100);
  Serial.println("START");
  delay(500);
  Serial.print("memory: \t");
  Serial.println(freeMemory());
  Wire.begin();
  //sensors.init();
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  delay(1000);

  //Map cmap(MAP_WIDTH, MAP_HEIGHT);
  //cmap.printMap();

  Map::debugMap(8,8, 0, 0);
  
  if(digitalRead(CALIBRATION_MODE_PIN)){
    for(;;){
      sensors.checkAllValues();
      delay(500);
    }
  }
}

void loop(){
  sensors.checkAllValues();
  delay(500);
}
