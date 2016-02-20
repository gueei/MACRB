#include <QueueList.h>
#include <StackArray.h>
#include "Configuration.h"
#include <MemoryFree.h>
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


Map::Map(unsigned char width, unsigned char height){
  mapWidth = width;
  mapHeight = height;
  for(int y=0; y<mapHeight; y++){
    for(int x=0; x<mapHeight; x++){
      tiles[x][y] = Maptile();
    } 
  }
}

void Map::setWall(unsigned char x, unsigned char y, Direction dir){
  if(dir==North){
    tiles[x][y].setWall(North,true);
    if (y>0) tiles[x][y-1].setWall(South,true);
  }
  if(dir==East){
    tiles[x][y].setWall(East,true);
    if (x<=MAP_MAX_WIDTH - 1) tiles[x+1][y].setWall(West,true);
  }
  if(dir==South){
    tiles[x][y].setWall(South,true);
    if (y<=MAP_MAX_WIDTH - 1) tiles[x][y+1].setWall(North,true);
  }
  if(dir==West){
    tiles[x][y].setWall(West,true);
    if (x>0) tiles[x-1][y].setWall(East,true);
  }
}

void Map::setWall(Coordinate coor, Direction dir){
  setWall(coor.x, coor.y, dir);
}

void Map::printMap(){
  Serial.println("Map:");
  for(int j=0; j<mapHeight; j++){
    for(int i=0; i<mapWidth; i++){
      Serial.print(tiles[i][j].type);
      Serial.print(" ");
      Serial.print(tiles[i][j].visits);
      Serial.print("\t");
      if (tiles[i][j].hasWall(East)) Serial.print("|");
      Serial.print("\t");
    }
    
    Serial.println();
    
    for(int i=0; i<mapWidth; i++){
      if (tiles[i][j].hasWall(South)) Serial.print("---");
      else Serial.print("   ");      
      Serial.print("\t");
      Serial.print(" \t");
    }
    
    Serial.println();
  }
}

StackArray <Coordinate> Map::findPath(Coordinate start, Direction currentDirection, Coordinate entrance){
  QueueList <Coordinate> Q; // Queue
  Coordinate t;
  Coordinate p; //present
  Coordinate n; //next
  boolean V[MAP_MAX_WIDTH][MAP_MAX_HEIGHT]; // Visited Set
  backTrack track[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];

  Serial.println(F("find path"));
  
  for(int i=0;i<mapWidth; i++)
      for(int j=0;j<mapHeight; j++)
        V[i][j] = false;
        
  V[start.x][start.y] = true;
  for(int j=0;j<mapWidth; j++){
        for(int k=0;k<mapHeight; k++){
          track[j][k].cost=255;
          track[j][k].prevTile.x = UNDEF_TILE;
          track[j][k].prevTile.y = UNDEF_TILE;
        }
  }
  track[start.x][start.y].cost = 0;
  track[start.x][start.y].facing = currentDirection;
  
  Q.push(start);
  while(!Q.isEmpty()){
    t = Q.pop();
    
    unsigned char x = t.x;
    unsigned char y = t.y;
    
    findAvailableTile(x,y,North,V,track,Q);
    findAvailableTile(x,y,South,V,track,Q);
    findAvailableTile(x,y,East,V,track,Q);
    findAvailableTile(x,y,West,V,track,Q);
  }
  
// FOR DEBUG
#if DEBUGLEVEL > 5
  for(int j=0;j<mapHeight; j++){
        for(int i=0;i<mapWidth; i++){
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
#endif

   Coordinate destination;
   destination = determineDestination(start, track);
   if(destination.x==UNDEF_TILE) destination = entrance;
#if DEBUGLEVEL > 5   
   printCoordinate(destination);
#endif

  unsigned char evalX = destination.x;
  unsigned char evalY = destination.y;
  
  StackArray <Coordinate> St; //Stack
  St.push(destination);
  
  while(!(evalX==start.x && evalY==start.y)){
    backTrack evaluating = track[evalX][evalY];
    St.push(evaluating.prevTile);
    evalX = evaluating.prevTile.x;
    evalY = evaluating.prevTile.y;
  }
  
  return St;
};

void Map::findAvailableTile(
  unsigned char cx, unsigned char cy, // Current Evaluating Tile 
  Direction dir, // Look at this direction
  boolean V[MAP_MAX_WIDTH][MAP_MAX_HEIGHT], 
  backTrack track[MAP_MAX_WIDTH][MAP_MAX_HEIGHT], 
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
  
  if (destx<0 || desty<0 || destx>mapWidth-1 || desty>mapHeight-1) return;

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
  int cost = cTrack.cost + tiles[destx][desty].type + orientCost;
  if (cost>250) dTrack.cost = 250;
  else dTrack.cost = cost;
  track[destx][desty] = dTrack;
}

Coordinate Map::determineDestination(Coordinate start, backTrack track[MAP_MAX_WIDTH][MAP_MAX_HEIGHT]){
  unsigned char smallest = 150;
  Coordinate output;
  output.x = UNDEF_TILE;
  output.y = UNDEF_TILE;
  for(char y=0; y<mapHeight; y++){
    for(char x=0; x<mapWidth; x++){
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

void Map::addVisit(Coordinate coor){
  char visits = tiles[coor.x][coor.y].visits + 1;
  visits < 15 ? tiles[coor.x][coor.y].visits = visits : visits = 15;
}

void Map::setBlackTile(Coordinate coor){
  tiles[coor.x][coor.y].type = TileType::black;
  tiles[coor.x][coor.y].visits = 15;
}

void Map::setVictim(Coordinate coor){
  tiles[coor.x][coor.y].type = TileType::victim;
}

boolean Map::hasVictim(Coordinate coor){
  return tiles[coor.x][coor.y].type == TileType::victim;
}

void Map::debugMap(unsigned char w, unsigned char h, unsigned char ex, unsigned char ey){
  Coordinate start;
  Direction dir;
  Map rmap = Map(w, h);
  start.x = ex;
  start.y = ey;

  rmap.tiles[1][0].type = TileType::black;
  rmap.tiles[1][0].visits = 15;
  rmap.tiles[4][0].type = TileType::black;
  rmap.tiles[4][0].visits = 15;
  rmap.setWall(0, 0,East);
  rmap.setWall(3, 0,East);
  rmap.setWall(7, 0,South);
  rmap.setWall(1, 1,East);
  rmap.setWall(3, 1,East);
  rmap.setWall(4, 1,East);
  rmap.setWall(5, 1,East);
  rmap.setWall(7, 1,East);
  rmap.setWall(2, 1,South);
  rmap.setWall(4, 1,South);
  rmap.setWall(7, 1,South);
  rmap.setWall(0, 2,East);
  rmap.setWall(3, 2,East);
  rmap.setWall(5, 2,East);
  rmap.setWall(8, 2,East);
  rmap.setWall(1, 2,South);
  rmap.setWall(5, 2,South);
  rmap.setWall(8, 2,South);
  rmap.setWall(2, 3,East);
  rmap.setWall(5, 3,East);
  rmap.setWall(6, 3,East);

  
  rmap.printMap();
  dir = North;
  
  do {
    Serial.print(F("Free Memory: \t"));
    Serial.println(freeMemory());

    Coordinate entrance;
    entrance.x = ex;
    entrance.y = ey;

    StackArray<Coordinate> pathStack = rmap.findPath(start, dir, entrance);
    rmap.tiles[start.x][start.y].visits++;  
 
    if (pathStack.count()<=1) break;
    
    pathStack.pop(); // the starting
    Coordinate next = pathStack.pop();

    Serial.print(F("New destination"));
    rmap.printCoordinate(next);
    Serial.println();

    rmap.printMap();
  
    delay(2000);
    if (start.x == next.x){
      if (start.y > next.y) dir = North;
      else dir = South;
    }else{
      if (start.x>next.x) dir = West;
      else dir = East;
    }
    start = next;   
  }while(true);
}
