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
    tiles[coor.x][coor.y-1].setWall(South,true);
  }
  if(dir==East){
    tiles[coor.x][coor.y].setWall(East,true);
    tiles[coor.x+1][coor.y].setWall(West,true);
  }
  if(dir==South){
    tiles[coor.x][coor.y].setWall(South,true);
    tiles[coor.x][coor.y-1].setWall(North,true);
  }
  if(dir==West){
    tiles[coor.x][coor.y].setWall(West,true);
    tiles[coor.x-1][coor.y].setWall(East,true);
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

/*void Map::findPath(Coordinate start, Coordinate dest, int initOrient){
  QueueList <Coordinate> Q; // Queue
  Coordinate t[MAP_WIDTH][MAP_HEIGHT];
  boolean V[MAP_WIDTH][MAP_HEIGHT]; // Visited Set
  for(int i=0;i<MAP_WIDTH; i++){
      for(int j=0;j<MAP_HEIGHT; j++){
        V[i][j] = false;
      }
  }
  V[start.x][start.y] = true;
  t[start.x][start.y].cost = 0;
  track[startx][starty].facing = initOrient;
  Q.push(start);
  while(!Q.isEmpty()){
    t = Q.pop();
    int x = t.x;
    int y = t.y;
    tile = Map[x][y];
    
    // North
    if (y>0 && !tile.nw && !V[x][y-1]){
      V[x][y-1] = true;
      Coord n;
      n.x = x;
      n.y = y-1;
      if(track[x][y].facing==0) {
        track[x][y].orientCost=0;
      }
      else if(track[x][y].facing==1 || track[x][y].facing==3){
        track[x][y].orientCost=1;
      }
      else{
        track[x][y].orientCost=2;
      };
      track[x][y-1].prevTile.x = x;
      track[x][y-1].prevTile.y = y;
      track[x][y-1].facing=0;
      track[x][y-1].cost = track[x][y].cost + Map[x][y].cost + track[x][y].orientCost;
      Q.push(n);
    }
    
    // South
    if(y<MapDimension - 1 && !tile.sw && !V[x][y+1]){
      V[x][y+1] = true;
      Coord n;
      n.x = x;
      n.y = y+1;
      if(track[x][y].facing==0) {
        track[x][y].orientCost=2;
      }
      else if(track[x][y].facing==1 || track[x][y].facing==3){
        track[x][y].orientCost=1;
      }
      else{
        track[x][y].orientCost=0;
      };
      track[x][y+1].prevTile.x = x;
      track[x][y+1].prevTile.y = y;
      track[x][y+1].facing=2;
      track[x][y+1].cost = track[x][y].cost + Map[x][y].cost + track[x][y].orientCost;
      Q.push(n);
    }
    
    //West
    if(x>0 && !tile.ww && !V[x-1][y]){
      V[x-1][y] = true;
      Coord n;
      n.x = x-1;
      n.y = y;
      if(track[x][y].facing==3) {
        track[x][y].orientCost=0;
      }
      else if(track[x][y].facing==0 || track[x][y].facing==2){
        track[x][y].orientCost=1;
      }
      else{
        track[x][y].orientCost=2;
      };
      track[x-1][y].prevTile.x = x;
      track[x-1][y].prevTile.y = y;
      track[x-1][y].facing=3;
      track[x-1][y].cost = track[x][y].cost + Map[x][y].cost + track[x][y].orientCost;
      Q.push(n);
    }
    
    //East
    if(x<MapDimension -1 && !tile.ew && !V[x+1][y]){
      V[x+1][y] = true;
      Coord n;
      n.x = x+1;
      n.y = y;
      if(track[x][y].facing==1) {
        track[x][y].orientCost=0;
      }
      else if(track[x][y].facing==0 || track[x][y].facing==2){
        track[x][y].orientCost=1;
      }
      else{
        track[x][y].orientCost=2;
      };
      track[x+1][y].prevTile.x = x;
      track[x+1][y].prevTile.y = y;
      track[x+1][y].facing=1;
      track[x+1][y].cost = track[x][y].cost + Map[x][y].cost + track[x][y].orientCost;
      Q.push(n);
    }
  }
  for(int i=0;i<MapDimension; i++){
        for(int j=0;j<MapDimension; j++){
          Serial.print(track[j][i].cost);
          Serial.print("(");
          Serial.print(track[j][i].prevTile.x);
          Serial.print(",");
          Serial.print(track[j][i].prevTile.y);
          Serial.print(")");
          Serial.print("\t");
        }
        Serial.println();
        Serial.println();
      }
    
    // Print track in reverse order
  int evalX=goalx;
  int evalY=goaly;
  
  while(!(evalX==startx && evalY==starty)){
    Serial.println("Evaluating:");
    Serial.print(evalX);
    Serial.print("\t");
    Serial.println(evalY);
   Serial.println("Forward 30");
   backTrack evaluating = track[evalX][evalY];
   if( track[evaluating.prevTile.x][evaluating.prevTile.y].facing == track[evalX][evalY].facing){
      Serial.println("No rotation");
       Serial.print("Facing of prevTile:  ");
      Serial.print(track[evaluating.prevTile.x][evaluating.prevTile.y].facing);
      Serial.print("\t");
       Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[evalX][evalY].facing);
      Serial.println();
     }
     else if(
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==1 && track[evalX][evalY].facing==0) ||
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==2 && track[evalX][evalY].facing==1) ||
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==3 && track[evalX][evalY].facing==2) ||
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==0 && track[evalX][evalY].facing==3)
     ){
       Serial.println("Turn left 90°");
       Serial.print("Facing of prevTile:  ");
      Serial.print(track[evaluating.prevTile.x][evaluating.prevTile.y].facing);
      Serial.print("\t");
       Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[evalX][evalY].facing);
      Serial.println();
     }
     else if(
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==0 && track[evalX][evalY].facing==1) ||
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==1 && track[evalX][evalY].facing==2) ||
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==2 && track[evalX][evalY].facing==3) ||
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==3 && track[evalX][evalY].facing==0)
     ){
       Serial.println("Turn right 90°");
       Serial.print("Facing of prevTile:  ");
      Serial.print(track[evaluating.prevTile.x][evaluating.prevTile.y].facing);
      Serial.print("\t");
       Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[evalX][evalY].facing);
      Serial.println();
     }
     else if(
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==0 && track[evalX][evalY].facing==2) ||
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==1 && track[evalX][evalY].facing==3) ||
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==2 && track[evalX][evalY].facing==0) ||
     (track[evaluating.prevTile.x][evaluating.prevTile.y].facing==3 && track[evalX][evalY].facing==1)
     ){
     Serial.println("Turn 180°");
       Serial.print("Facing of prevTile:  ");
      Serial.print(track[evaluating.prevTile.x][evaluating.prevTile.y].facing);
      Serial.print("\t");
       Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[evalX][evalY].facing);
      Serial.println();
     }
     else{ Serial.println("Error");
       Serial.print("Facing of prevTile:  ");
      Serial.print(track[evaluating.prevTile.x][evaluating.prevTile.y].facing);
      Serial.print("\t");
       Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[evalX][evalY].facing);
      Serial.println();
     };
     evalX = evaluating.prevTile.x;
     evalY = evaluating.prevTile.y;
  }
  
     
     };*/
