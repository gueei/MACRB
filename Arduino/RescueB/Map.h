
#include <Arduino.h>
#include <inttypes.h>
#include "Configuration.h"
#include <QueueList.h>
#include <StackArray.h>

enum TileType{
  normal = 1,
  black = 200,
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
  unsigned x:4, y:4;
};

struct backTrack{
  Coordinate prevTile;
  unsigned char cost;
  Direction facing;
};

class Maptile{
  public:
    Maptile();  // Constructor
    TileType type;
    unsigned visits:4;
    unsigned walls:4;
    void setWall(Direction dir, boolean on); //only one wall
    boolean hasWall(Direction dir);
  private:
};

class Map{
  public:
    Map(unsigned char width, unsigned char height);
    Maptile tiles[MAP_MAX_WIDTH][MAP_MAX_HEIGHT];
    void setWall(Coordinate coor, Direction dir);// both walls at once
    void setWall(unsigned char x, unsigned char y, Direction dir);//same as above but different way to input parameters
    void printMap();
    void addVisit(Coordinate coor);
    StackArray <Coordinate> findPath(Coordinate start, Direction currentDirection, Coordinate entrance);
    void printCoordinate(Coordinate coor);
    static void debugMap(int w, int h, int ex, int ey);
  private:
    int mapWidth, mapHeight;
    void findAvailableTile(unsigned char cx, unsigned char cy, Direction dir, 
      boolean V[MAP_MAX_WIDTH][MAP_MAX_HEIGHT], backTrack track[MAP_MAX_WIDTH][MAP_MAX_HEIGHT], QueueList<Coordinate>& Q);
    Coordinate determineDestination(Coordinate start, backTrack track[MAP_MAX_WIDTH][MAP_MAX_HEIGHT]);
};
