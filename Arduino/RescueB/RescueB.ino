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
  Wire.begin();
  
  Serial.begin(115200);
  delay(100);
  Serial.println(F("START"));
  delay(500);

  Serial.println(F("Sensor Initialization"));
  sensors.init();

  Serial.println(F("Sensor Init complete"));
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  delay(1000);

  //Map cmap(MAP_WIDTH, MAP_HEIGHT);
  //cmap.printMap();

  //Map::debugMap(8,4, 0, 0);
}

void loop(){
  Serial.print("heading\t");
  Serial.println(sensors.getHeading());

  Serial.println(sensors.getRange());
  Serial.println(sensors.getTemperatureRight());
  
  delay(1000);
}
