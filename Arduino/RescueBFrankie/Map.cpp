#include "Map.h"

// Constructor
Maptile::Maptile(){
  walls = 0x00;
  type = normal;
}

boolean Maptile::hasWall(Direction dir){
  return walls & dir;
}

void Maptile::setWall(Direction dir, boolean on){
  if (on){
    walls |= dir;
  }
}

Map::Map(){
  for(int y=0; y<MAP_HEIGHT; y++){
    for(int x=0; x<MAP_WIDTH; x++){
      tiles[x][y] = Maptile();
    } 
  }
}

void Map::printMap(){
  
}
