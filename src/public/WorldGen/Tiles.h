#pragma once
#include "CoreTypes.h"

class TileType;

enum class Direction
{
  North,
  South,
  East,
  West
};

struct Tile
{
  RenderData *rdata;
  TileType *type;
};

/* Typed Obj
 * Handles tile behaviour, such as the effect the tile performs
 * and the logic determining if an actor can move to that tile;
 */
class TileType
{
public:
  virtual bool CanMoveTo(Actor *actor) = 0;
  virtual void Effect() = 0;
};

class FloorTile : public TileType
{
public:
  virtual bool CanMoveTo(Actor *actor) override { return true; }
  virtual void Effect() override { FloorTileEffect(); };

protected:
  virtual void FloorTileEffect() {};
};

class WallTile : public TileType
{
public:
  virtual bool CanMoveTo(Actor *actor) override { return false; }
  virtual void Effect() override {};
};

class Grid
{
public:
  const IntVector origin;

  Grid(IntVector inOrigin);
  virtual ~Grid();

  bool GetTile(IntVector coord, Tile &out);
  bool UpdateTile(IntVector coord, Tile &in);
  bool UpdateTiles(IntVector from, IntVector to, Tile &in);
  void DrawTick(float delta);

private:
  std::unordered_map<IntVector, Tile> grid;
};
