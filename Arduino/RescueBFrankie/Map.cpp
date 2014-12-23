#include <QueueList.h>
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

void Map::setWall(Coordinate coor, Direction dir){
  if(dir==North){
    tiles[coor.x][coor.y].setWall(North,true);
    tiles[coor.x-1][coor.y].setWall(South,true);
  }
  if(dir==East){
    tiles[coor.x][coor.y].setWall(East,true);
    tiles[coor.x][coor.y+1].setWall(West,true);
  }
  if(dir==South){
    tiles[coor.x][coor.y].setWall(South,true);
    tiles[coor.x+1][coor.y].setWall(North,true);
  }
  if(dir==West){
    tiles[coor.x][coor.y].setWall(West,true);
    tiles[coor.x][coor.y-1].setWall(East,true);
  }
}

void Map::printMap(){
  Serial.println("Map:");
  for(int j=0; j<MAP_HEIGHT; j++){
    for(int i=0; i<MAP_WIDTH; i++){
      Serial.print(tiles[i][j].type);
      if (tiles[i][j].hasWall(East)) Serial.print("|");
      else Serial.print("\t");
    }
    
    Serial.println();
    
    for(int i=0; i<MAP_WIDTH; i++){
      if (tiles[i][j].hasWall(South)) Serial.print("- ");
      else Serial.print("\t");
    }
    
    Serial.println();
  }
}

void findPath(Coordinate start, Coordinate dest, int initOrient){
  QueueList <Coordinate> Q; // Queue
  
     
     };
