#define DEBUGLEVEL 4

#include <QueueList.h>
#include <StackArray.h>
#include "Map.h"

Coordinate start;
  Map rmap = Map();

void setup(){
  Serial.begin(115200);
  int initOrient = 1;
  
  rmap.setWall(2, 3,South);
  rmap.setWall(0, 0,South);
  rmap.tiles[3][4].type = black;
  rmap.tiles[3][0].type = speedbump;
  rmap.printMap();
  start.x = 0;
  start.y = 0;
  rmap.findPath(start, North);
}

void loop(){
  /*rmap.tiles[start.x][start.y].visits = 1;
 
  start = rmap.findPath(start, North);
  Serial.print("New destination");
  rmap.printCoordinate(start);
  Serial.println();
  
  delay(5000);
  */
}
