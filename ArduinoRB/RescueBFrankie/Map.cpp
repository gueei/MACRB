#include <QueueList.h>
#include <StackArray.h>
#include "Map.h"

// Constructor
Maptile::Maptile(){
  walls = 0x00;
  type = normal;
  visits = 0;
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

void Map::setWall(int x, int y, Direction dir){
  if(dir==North){
    tiles[x][y].setWall(North,true);
    tiles[x][y-1].setWall(South,true);
  }
  if(dir==East){
    tiles[x][y].setWall(East,true);
    tiles[x+1][y].setWall(West,true);
  }
  if(dir==South){
    tiles[x][y].setWall(South,true);
    tiles[x][y-1].setWall(North,true);
  }
  if(dir==West){
    tiles[x][y].setWall(West,true);
    tiles[x-1][y].setWall(East,true);
  }
}

void Map::setWall(Coordinate coor, Direction dir){
  setWall(coor.x, coor.y, dir);
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

Coordinate Map::findPath(Coordinate start, Direction currentDirection){
  QueueList <Coordinate> Q; // Queue
  StackArray <Coordinate> St; //Stack
  Coordinate t;
  Coordinate p; //present
  Coordinate n; //next
  boolean V[MAP_WIDTH][MAP_HEIGHT]; // Visited Set
  backTrack track[MAP_WIDTH][MAP_HEIGHT];
  for(int i=0;i<MAP_WIDTH; i++)
      for(int j=0;j<MAP_HEIGHT; j++)
        V[i][j] = false;
  V[start.x][start.y] = true;
  for(int j=0;j<MAP_WIDTH; j++){
        for(int k=0;k<MAP_HEIGHT; k++){
          track[j][k].cost=999;
          track[j][k].prevTile.x = -1;
          track[j][k].prevTile.y = -1;
        }
  }
  track[start.x][start.y].cost = 0;
  track[start.x][start.y].facing = currentDirection;
  
  Q.push(start);
  while(!Q.isEmpty()){
    t = Q.pop();
    
    int x = t.x;
    int y = t.y;
    
    findAvailableTile(x,y,North,V,track,Q);
    findAvailableTile(x,y,South,V,track,Q);
    findAvailableTile(x,y,East,V,track,Q);
    findAvailableTile(x,y,West,V,track,Q);
  }
  
// FOR DEBUG
  for(int j=0;j<MAP_HEIGHT; j++){
        for(int i=0;i<MAP_WIDTH; i++){
          Serial.print(track[i][j].cost);
          Serial.print("(");
          Serial.print(track[i][j].prevTile.x);
          Serial.print(",");
          Serial.print(track[i][j].prevTile.y);
          Serial.print(")");
          Serial.print("\t");
        }
        Serial.println();
        Serial.println();
   }
   
   Coordinate destination;
   destination.x=3;
   destination.y=3;
   //determineDestination(start, track);
   printCoordinate(destination);
   Serial.println();
   
   //return destination;

  int evalX=destination.x;
  int evalY=destination.y;
  
  St.push(destination);
  
  while(!(evalX==start.x && evalY==start.y)){
  backTrack evaluating = track[evalX][evalY];
  
     evalX = evaluating.prevTile.x;
     evalY = evaluating.prevTile.y;
     
     St.push(evaluating.prevTile);
  }
  
  while(!St.isEmpty()){
    p = St.pop();
    n = St.peek();
    
  backTrack evaluating = track[n.x][n.y];
    Serial.println("Path taken:");
    Serial.print(p.x);
    Serial.print("\t");
    Serial.println(p.y);
    Serial.println("Forward 30");
    
    if( track[n.x][n.y].facing == track[p.x][p.y].facing){
      Serial.println("No rotation");
      Serial.print("Facing of current tile:  ");
      Serial.print(track[p.x][p.y].facing);
      Serial.print("\t");
      Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[n.x][n.y].facing);
      Serial.println();
     }
     else if(
     (track[p.x][p.y].facing==East && track[n.x][n.y].facing==North) ||
     (track[p.x][p.y].facing==South && track[n.x][n.y].facing==East) ||
     (track[p.x][p.y].facing==West && track[n.x][n.y].facing==South) ||
     (track[p.x][p.y].facing==North && track[n.x][n.y].facing==West)
     ){
      Serial.println("Turn left 90°");
      Serial.print("Facing of current tile:  ");
      Serial.print(track[p.x][p.y].facing);
      Serial.print("\t");
       Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[n.x][n.y].facing);
      Serial.println();
     }
     else if(
     (track[p.x][p.y].facing==North && track[n.x][n.y].facing==East) ||
     (track[p.x][p.y].facing==East && track[n.x][n.y].facing==South) ||
     (track[p.x][p.y].facing==South && track[n.x][n.y].facing==West) ||
     (track[p.x][p.y].facing==West && track[n.x][n.y].facing==North)
     ){
       Serial.println("Turn right 90°");
      Serial.print("Facing of current tile:  ");
      Serial.print(track[p.x][p.y].facing);
      Serial.print("\t");
       Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[n.x][n.y].facing);
      Serial.println();
     }
     else if(
     (track[p.x][p.y].facing==North && track[n.x][n.y].facing==South) ||
     (track[p.x][p.y].facing==East && track[n.x][n.y].facing==West) ||
     (track[p.x][p.y].facing==South && track[n.x][n.y].facing==North) ||
     (track[p.x][p.y].facing==West && track[n.x][n.y].facing==East)
     ){
     Serial.println("Turn 180°");
      Serial.print("Facing of current tile:  ");
      Serial.print(track[p.x][p.y].facing);
      Serial.print("\t");
       Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[n.x][n.y].facing);
      Serial.println();
     }
     else{ Serial.println("Error");
      Serial.print("Facing of current tile:  ");
      Serial.print(track[p.x][p.y].facing);
      Serial.print("\t");
       Serial.print("Facing of evaluating tile:  ");
      Serial.println(track[n.x][n.y].facing);
      Serial.println();
     };
    
    
  };  
};

void Map::findAvailableTile(
  int cx, int cy, // Current Evaluating Tile 
  Direction dir, // Look at this direction
  boolean V[MAP_WIDTH][MAP_HEIGHT], 
  backTrack track[MAP_WIDTH][MAP_HEIGHT], 
  QueueList<Coordinate>& Q
){
  int destx = cx, desty = cy;
  switch(dir){
    case North:
      desty=cy-1; break;
    case East:
      destx=cx+1; break;
    case South:
      desty=cy+1; break;
    case West:
      destx=cx-1; break;
  }
  
  if (destx<0 || desty<0 || destx>MAP_WIDTH-1 || desty>MAP_HEIGHT-1) return;

  if (tiles[cx][cy].hasWall(dir) || V[destx][desty]) return;

  V[destx][desty] = true;
  Coordinate n;
  n.x = destx;
  n.y = desty;  
  Q.push(n);
  
  backTrack cTrack = track[cx][cy]; //current
  backTrack dTrack = track[destx][desty];
  int orientCost = 0;
  if(cTrack.facing==dir) {
    orientCost=0;
  }else{
    if ((dir>South && cTrack.facing > South) ||
      (dir<=South && cTrack.facing <=South)) 
      orientCost = 2;
    else
      orientCost = 1;
  }
  
  dTrack.prevTile.x = cx;
  dTrack.prevTile.y = cy;
  dTrack.facing=dir;
  dTrack.cost = cTrack.cost + tiles[destx][desty].type + orientCost;
  track[destx][desty] = dTrack;
}

Coordinate Map::determineDestination(Coordinate start, backTrack track[MAP_WIDTH][MAP_HEIGHT]){
  int smallest = 999999;
  Coordinate output;
  output.x = -1;
  output.y = -1;
  for(int y=0; y<MAP_HEIGHT; y++){
    for(int x=0; x<MAP_WIDTH; x++){
      if (track[x][y].cost < smallest){
        if (start.x == x && start.y == y) continue;
        if (tiles[x][y].visits > 0) continue;
        output.x = x;
        output.y = y;
        smallest = track[x][y].cost;
      }
    }
  }
  return output;
}

void Map::printCoordinate(Coordinate coor){
  Serial.print("(");
  Serial.print(coor.x);
  Serial.print(", ");
  Serial.print(coor.y);
  Serial.print(")");
}
