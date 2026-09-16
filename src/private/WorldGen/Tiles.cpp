#include "WorldGen/Tiles.h"
#include "CoreUtil.h"

/*===========================================================*/
//	        		Grid
/*===========================================================*/

Grid::Grid(IntVector inOrigin) : origin(inOrigin) {}
Grid::~Grid() {}

bool Grid::GetTile(IntVector coord, Tile &outTile)
{
  if(!grid.contains(coord))
    return false;
  outTile = grid[coord];
  return true;
};

void Grid::DrawTick(float delta)
{
  GlobalSettings *settings = GlobalSettings::GetSettings();
  for(auto &pair : grid)
    {
      RenderData *data = pair.second.rdata;
      if(!data->initalizedTruePosition)
        data->ApplyPosition(CoreUtil::GetTrueCoordinates(this, pair.first));

      data->DrawSprite();
    }

  // these are grid debug lines
  for(int x = 0; x < settings->screenWidth; x += 16)
    {
      DrawLineDashed({(float)x, 0}, {(float)x, (float)settings->screenHeight},
                     4, 4, RED);
      for(int y = 0; y < settings->screenHeight; y += 16)
        {
          DrawLineDashed({0, (float)y},
                         {(float)settings->screenWidth, (float)y}, 4, 4, RED);
        }
    }
}

// need to write some functions that apply int vectors to tile coordinates
// this will explicitly rely when we are working on a tile position and when we
// are working with a force
//
//
// maybe the grid is like a crawler and the data underneath is a server??
// that could be cool

bool Grid::UpdateTile(IntVector coord, Tile &in)
{
  IntVector local = coord - origin;
  local.x = std::abs(local.x);
  local.y = std::abs(local.y);

  RenderData *inData = in.rdata;
  RenderData *gridData = grid.at(local).rdata;
  inData->ApplyPosition(CoreUtil::GetTrueCoordinates(this, local));
  gridData->Apply(inData);

  return true;
}

bool Grid::UpdateTiles(IntVector from, IntVector to, Tile &in) { return true; }
