#define  MAP_WIDTH    4
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
  East = 0x02,
  South = 0x04,
  West = 0x08
};

struct Coordinate{
  int x, y;
};

class Maptile{
  public:
    Maptile();  // Constructor
    TileType type;
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
    void printMap();//2
    void findPath(Coordinate start, Coordinate dest, int initOrient);//3
  private:
    
};
