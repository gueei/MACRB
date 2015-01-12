
#include <Arduino.h>
#include <inttypes.h>
#include "Configuration.h"
#include <QueueList.h>
#include <StackArray.h>

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
    void setWall(Direction dir, boolean on); //only one wall
    boolean hasWall(Direction dir);
    byte walls;
  private:
};

class Map{
  public:
    Map(int width, int height);
    Maptile tiles[MAP_WIDTH][MAP_HEIGHT];
    void setWall(Coordinate coor, Direction dir);// both walls at once
    void setWall(int x, int y, Direction dir);//same as above but different way to input parameters
    void printMap();
    void addVisit(Coordinate coor);
    StackArray <Coordinate> findPath(Coordinate start, Direction currentDirection, Coordinate entrance);
    void printCoordinate(Coordinate coor);
    static void debugMap(int ex, int ey);
  private:
    void findAvailableTile(int cx, int cy, Direction dir, 
      boolean V[MAP_WIDTH][MAP_HEIGHT], backTrack track[MAP_WIDTH][MAP_HEIGHT], QueueList<Coordinate>& Q);
    Coordinate determineDestination(Coordinate start, backTrack track[MAP_WIDTH][MAP_HEIGHT]);
};
