#include "CoreTypes.h"
#include "CoreUtil.h"
#include <raylib.h>

/*===========================================================*/
//		      Global Settings
/*===========================================================*/
GlobalSettings *GlobalSettings::instance = nullptr;

GlobalSettings::GlobalSettings() {}

const ISystem *GlobalSettings::GetSystem()
{ return GlobalSettings::GetSettings(); }

GlobalSettings *GlobalSettings::GetSettings()
{
  if(!GlobalSettings::instance)
    {
      GlobalSettings::instance = new GlobalSettings;
    }
  return GlobalSettings::instance;
}

/*===========================================================*/
//		        Render Data
/*===========================================================*/

bool RenderData::Apply(RenderData::Internal &&data)
{
  sprite = std::move(data);
  return true;
}

bool RenderData::Apply(RenderData &data)
{
  sprite = data.sprite;
  return true;
}

bool RenderData::Apply(Vector2 &&pos)
{
  sprite.position = pos;
  initalizedTruePosition = true;
  return true;
}

bool RenderData::DrawSprite() const
{
  DrawTextureRec(sprite.texture, sprite.frame, sprite.position, WHITE);
  return true;
}

/*===========================================================*/
//		        Render System
/*===========================================================*/

RenderSystem *RenderSystem::instance = nullptr;

RenderSystem::RenderSystem() {}

RenderSystem *RenderSystem::GetRenderSystem()
{
  if(!RenderSystem::instance)
    {
      RenderSystem::instance = new RenderSystem;
    }
  return RenderSystem::instance;
}

const ISystem *RenderSystem::GetSystem()
{ return RenderSystem::GetRenderSystem(); }

void RenderSystem::CallDrawTick()
{
  // we could just put the filling the memory as its own thread
  // i would likley benefit from also putting the drawing loop on that thread
  // to for better animation coordination but for now i'll keep it easier
  for(int layer = 0; layer < RENDER_STACK_LENGTH; layer++)
    {
      std::span<RenderData> span(renderStack[layer]);
    }
};

void RenderSystem::Draw()
{
  struct MetaData
  {
    float deltaTime;
    int numOfSkippedCycles;
  };
  MetaData mean[RENDER_STACK_LENGTH];
  while(!WindowShouldClose())
    {
      // cycle
      for(int layer = 0; layer < RENDER_STACK_LENGTH; layer++)
        {
          for(RenderData &data : renderStack[layer])
            {
              if(!data.DrawSprite())
                {
                  data.deltaTime += 0.01f; // get real delta
                  data.numOfSkippedCycles++;
                }
              mean[layer].deltaTime += data.deltaTime;
              mean[layer].numOfSkippedCycles += data.numOfSkippedCycles;
            }
          mean[layer].deltaTime = mean[layer].deltaTime / RENDER_STACK_LENGTH;
          mean[layer].numOfSkippedCycles
              = int(mean[layer].numOfSkippedCycles / RENDER_STACK_LENGTH);
        }
      // find the next layer to work on
      for(auto data : mean) {}
      // for each tile
      // we check its mutex
      // and update the meta data/ internal data accordingly
      //	we also keep track of the cycles meta data avrg per layer
      //	so we have data to base what layer needs an update next
    }
};

/*===========================================================*/
//	        		Grid
/*===========================================================*/

Grid::Grid(SpriteLayout *layout, TileCoordinate &inOrigin,
           IntVector &inDirection)
    : origin(inOrigin), direction(inDirection), grid(layout)
{}
Grid::Grid(SpriteLayout *layout, TileCoordinate &&inOrigin,
           IntVector &&inDirection)
    : origin(inOrigin), direction(inDirection), grid(layout)
{}

Grid::~Grid() {}

bool Grid::GetTileRenderData(TileCoordinate coord, RenderData &out)
{
  if(!WithinGrid(coord))
    return false;

  return true;
}

void Grid::DrawTick(std::span<RenderData> &drawStack, float delta)
{
  GlobalSettings *settings = GlobalSettings::GetSettings();
  for(auto &pair : *grid)
    {
      if(!pair.second.initalizedTruePosition)
        pair.second.Apply(CoreUtil::GetTrueCoordinates(this, pair.first));

      pair.second.DrawSprite();
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

bool Grid::UpdateTile(TileCoordinate coord, RenderData &in)
{
  TileCoordinate local = coord - origin;
  local.x = std::abs(local.x);
  local.y = std::abs(local.y);
  in.Apply(CoreUtil::GetTrueCoordinates(this, local));
  grid->at(local).Apply(in);
  return true;
}

bool Grid::UpdateTiles(TileCoordinate from, TileCoordinate to, RenderData &in)
{ return true; }

bool Grid::WithinGrid(TileCoordinate coord) { return true; }

bool Grid::ExpandGridTo(TileCoordinate coord) { return true; }
