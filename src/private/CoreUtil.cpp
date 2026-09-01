#include "CoreUtil.h"
#include "CoreTypes.h"
#include <raylib.h>

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

Vector2 CoreUtil::GetTrueCoordinates(Grid *grid, const TileCoordinate &coord)
{
  float tileSize = GlobalSettings::GetSettings()->tileSize;

  TileCoordinate local = coord - grid->origin;
  return {grid->origin.x * tileSize + (grid->direction.x * local.x * tileSize),
          grid->origin.y * tileSize
              + (grid->direction.y * local.y * tileSize)};
}

Vector2
CoreUtil::GetCenterTrueCoordinates(Grid *grid, const TileCoordinate &coord)
{
  float tileSize = GlobalSettings::GetSettings()->tileSize;

  Vector2 topLeft = GetTrueCoordinates(grid, coord); // Pass the grid!

  Vector2 halfExtents = {grid->direction.x * (tileSize / 2),
                         grid->direction.y * (tileSize / 2)};

  return {topLeft.x + halfExtents.x, topLeft.y + halfExtents.y};
}

SpriteLayout
CoreUtil::CreateRenderDataFromSpriteSheet(Texture2D &texture, float cellSize)
{
  SpriteLayout data;
  float cellHeight = texture.height / cellSize;
  float cellWidth = texture.width / cellSize;

  data.reserve(int(cellHeight * cellWidth));

  for(int w = 0; w < cellWidth; w++)
    {
      for(int h = 0; h < cellHeight; h++)
        {
          data[{w, h}] = RenderData({texture,
                                     {float(w) * cellSize, float(h) * cellSize,
                                      cellSize, cellSize}});
        }
    }
  return data;
};
