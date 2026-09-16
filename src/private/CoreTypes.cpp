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

bool RenderData::ApplyInternal(Sprite &&newSprite)
{
  sprite = std::move(newSprite);
  return true;
}

bool RenderData::Apply(RenderData &data)
{
  sprite = data.sprite;
  return true;
}

bool RenderData::Apply(RenderData &&data)
{
  sprite = data.sprite;
  return true;
}

bool RenderData::Apply(RenderData *data)
{
  sprite = data->sprite;
  return true;
}

bool RenderData::DrawSprite() const
{
  DrawTextureRec(sprite.texture, sprite.frame, renderPos, WHITE);
  return true;
}

/*===========================================================*/
//		       Sprite Atlas
/*===========================================================*/

SpriteAtlas::SpriteAtlas(Texture2D *sheet) : spriteSheet(sheet)
{
  float cellHeight = (float)spriteSheet->height / TILE_SIZE;
  float cellWidth = (float)spriteSheet->width / TILE_SIZE;

  atlas.reserve(int(cellHeight * cellWidth));

  for(int w = 0; w < cellWidth; w++)
    {
      for(int h = 0; h < cellHeight; h++)
        {
          atlas[{w, h}] = Sprite{spriteSheet,
                                 {float(w) * TILE_SIZE, float(h) * TILE_SIZE,
                                  TILE_SIZE, TILE_SIZE}};
        }
    }
}

const Sprite *const SpriteAtlas::GetSpritePtr(IntVector coord)
{
  if(!atlas.contains(coord))
    return nullptr;

  return &atlas[coord];
};

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
      std::span<RenderData> span(renderMemory[layer]);
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
          for(RenderData &data : renderMemory[layer])
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
