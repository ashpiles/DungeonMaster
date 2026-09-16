#include "CoreTypes.h"
#include "CoreUtil.h"
#include "WorldGen/Tiles.h"

/*
 * do I need to create a unique asset file type to encode collisions into the
 * tile map? no i'll just use a second tilemap to define collisions easy
 *
 * wait we are making moves like chess pieces so we don't move standardly
 * how do we determine a legal move?
 * for example if we only move in a L or diagonally
 *
 * part of the desing of chess is that we have two forces meeting, which means
 * the movement restriction of pieces is intersting the terrain of chess is 2D,
 * I want to make it pseudo 3d this just means I have more types of pieces!
 *
 * I'll create an enum and relate it to a color, this will encode the different
 * movement costs/ types
 *
 * terrain types:
 * floor/slope (universal)
 * wall
 * air
 * water
 * difficult
 *
 * You attack by picking up a piece and placing it
 * placing a piece in place taps for an effect
 *
 *
 * so the issue now is that
 */

Vector2 CoreUtil::GetTrueCoordinates(Grid *grid, const IntVector &coord)
{
  float tileSize = GlobalSettings::GetSettings()->tileSize;

  IntVector local = coord - grid->origin;
  return {grid->origin.x * tileSize + (local.x * tileSize),
          grid->origin.y * tileSize + (local.y * tileSize)};
}

Vector2 CoreUtil::GetCenterTrueCoordinates(Grid *grid, const IntVector &coord)
{
  float tileSize = GlobalSettings::GetSettings()->tileSize;

  Vector2 topLeft = GetTrueCoordinates(grid, coord); // Pass the grid!

  Vector2 halfExtents = {(tileSize / 2), (tileSize / 2)};

  return {topLeft.x + halfExtents.x, topLeft.y + halfExtents.y};
}
