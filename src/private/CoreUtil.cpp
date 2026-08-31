#include "CoreUtil.h"
#include "CoreTypes.h"

Vector2 CoreUtil::GetTrueCoordinates(Grid *grid, TileCoordinate &coord)
{
  float tileSize = GlobalSettings::GetSettings()->tileSize;

  TileCoordinate local = coord - grid->origin;
  return {grid->origin.x * tileSize + (grid->direction.x * local.x * tileSize),
          grid->origin.y * tileSize
              + (grid->direction.y * local.y * tileSize)};
}

Vector2 CoreUtil::GetCenterTrueCoordinates(Grid *grid, TileCoordinate &coord)
{
  float tileSize = GlobalSettings::GetSettings()->tileSize;

  Vector2 topLeft = GetTrueCoordinates(grid, coord); // Pass the grid!

  Vector2 halfExtents = {grid->direction.x * (tileSize / 2),
                         grid->direction.y * (tileSize / 2)};

  return {topLeft.x + halfExtents.x, topLeft.y + halfExtents.y};
}

std::vector<RenderData>
CoreUtil::CreateRenderDataFromSpriteSheet(Texture2D &texture, float cellSize)
{
  std::vector<RenderData> data;
  float cellHeight = texture.height / cellSize;
  float cellWidth = texture.width / cellSize;

  data.reserve(int(cellHeight * cellWidth));
  for(int w = 0; w < cellWidth; w++)
    {
      for(int h = 0; h < cellHeight; h++)
        {
          data.emplace_back(
              RenderData({texture,
                          {float(w) * cellSize, float(h) * cellSize, cellSize,
                           cellSize}}));
        }
    }
  return data;
};
