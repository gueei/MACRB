#include "Map.h"

void setup(){
  Serial.begin(115200);
  Map rmap = Map();
  
  rmap.tiles[0][0].setWall(North, true);
  rmap.tiles[0][0].type = black;
  rmap.tiles[0][0].type = black;
}

void loop(){
  
}
