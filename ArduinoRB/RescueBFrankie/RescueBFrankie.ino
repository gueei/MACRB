#include <QueueList.h>
#include "Map.h"

void setup(){
  Serial.begin(115200);
  int initOrient = 1;
  Map rmap = Map();
  
  //rmap.tiles[2][2].setWall(South, true);
  Coordinate coor[MAP_WIDTH][MAP_HEIGHT];
  coor[2][3].x=2;
  coor[2][3].y=3;
  rmap.setWall(coor[2][3],South);
  rmap.tiles[3][4].type = black;
  rmap.tiles[3][0].type = speedbump;
  rmap.printMap();
}

void loop(){
  
}
