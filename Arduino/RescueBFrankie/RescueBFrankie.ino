#include <QueueList.h>
#include "Map.h"

void setup(){
  Serial.begin(115200);
  initOrient = 1;
  Map rmap = Map();
  
  Coordinate coor[MAP_WIDTH][MAP_HEIGHT];
  coor[2][3].x=3;
  coor[2][3].y=3;
  Map().setWall(coor[2][3],North);
  rmap.tiles[3][4].type = black;
  rmap.tiles[3][0].type = speedbump;
  rmap.printMap();
}

void loop(){
  
}
