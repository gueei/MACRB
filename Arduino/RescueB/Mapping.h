#include "Configuration.h"

enum Direction{
  North = 1,
  East = 1<<1,
  South = 1<<2,
  West = 1<<3
};

enum TileType{
  StartTile,
  SaveTile,
  NoEntryTile,
  NormalTile,
  DebrisTile,
  SpeedBumpTile_N,
  SpeedBumpTile_E,
  SpeedBumpTile_W,
  SpeedBumpTile_S
};

class MapTile{
  public:
    MapTile();
    int visits;
    byte wall;
    TileType type;
    boolean hasWall(Direction);
};

class Map{
  public:
    Map(int width, int height);  // Constructor
    void saveMap();  // Serialize map and save in.. maybe EEPROM?
    void loadMap();
    
    void putWall(int x, int y, Direction dir);
    int width, height;
    MapTile getTile(int x, int y);
};
