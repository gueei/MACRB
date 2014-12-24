#define  MAP_WIDTH    5
#define  MAP_HEIGHT   7

#include <Arduino.h>
#include <inttypes.h>
#include <QueueList.h>

enum TileType{
  normal = 1,
  black = 9999,
  victim = 2,
  debris = 10,
  speedbump = 20,
  obstacle = 11
};

enum Direction{
  North = 0x01,
  East = 0x04,
  South = 0x02,
  West = 0x08
};

struct Coordinate{
  int x, y;
};

struct backTrack{
  Coordinate prevTile;
  int cost;
  Direction facing;
};

class Maptile{
  public:
    Maptile();  // Constructor
    TileType type;
    int visits;
    void setWall(Direction dir, boolean on);
    boolean hasWall(Direction dir);
    byte walls;
  private:
};

class Map{
  public:
    Map();
    Maptile tiles[MAP_WIDTH][MAP_HEIGHT];
    void setWall(Coordinate coor, Direction dir);//1
    void setWall(int x, int y, Direction dir);
    void printMap();//2
    Coordinate findPath(Coordinate start, Direction currentDirection);
    //Maptile prevTile[MAP_WIDTH][MAP_HEIGHT];
    void printCoordinate(Coordinate coor);
  private:
    void findAvailableTile(int cx, int cy, Direction dir, 
      boolean V[MAP_WIDTH][MAP_HEIGHT], backTrack track[MAP_WIDTH][MAP_HEIGHT], QueueList<Coordinate>& Q);
    Coordinate determineDestination(Coordinate start, backTrack track[MAP_WIDTH][MAP_HEIGHT]);
};
