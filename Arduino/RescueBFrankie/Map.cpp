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

void Map::tSetWall(Coordinate coor, Direction dir){
  if(dir==0x01){
    tWalls |= dir;
    tiles[coor.x-1][coor.y].walls  |= 0x04;
  }
  if(dir==0x02){
    tWalls |= dir;
    tiles[coor.x][coor.y+1].walls |=0x08;
  }
  if(dir==0x04){
    tWalls |= dir;
    tiles[coor.x+1][coor.y].walls |=0x01;
  }
  if(dir==0x08){
    tWalls |= dir;
    tiles[coor.x][coor.y-1].walls |=0x02;
  }
}

void Map::printMap(){
  for(int j=0; j<MAP_HEIGHT; j++){
    for(int i=0; i<MAP_WIDTH; i++){
      Serial.print(tiles[i][j].type);
      if (tiles[i][j].walls==0x02) Serial.print("|");
      else Serial.print(" ");
    }
    
    Serial.println();
    
    for(int i=0; i<MAP_WIDTH; i++){
      if (tiles[i][j].walls==0x04) Serial.print("- ");
      else Serial.print("  ");
    }
    
    Serial.println();
  }
}
