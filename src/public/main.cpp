#include "CoreTypes.h"
#include "CoreUtil.h"
#include "raylib.h"

/*
 * 1 minute bullet chess,
 * but you unlock pieces types like a tier tree
 *
 * fly weight for the tile assets
 */

int main(void)
{
  GlobalSettings *Settings = GlobalSettings::GetSettings();
  RenderSystem *Renderer = RenderSystem::GetRenderSystem();
  Settings->screenHeight = 900;
  Settings->screenWidth = 1600;
  Settings->windowName = "DungeonMaster_V0.0.1";
  Settings->targetFPS = 60;
  Settings->tileSize = 16;

  InitWindow(Settings->screenWidth, Settings->screenHeight,
             Settings->windowName.c_str());
  Texture2D floorSpriteSheet
    = LoadTexture("/home/ash/Workspace/dev/DungeonMaster/content/Tiled_files/"
                  "walls_floor.png");
  std::vector<RenderData> renderData
    = CoreUtil::CreateRenderDataFromSpriteSheet(floorSpriteSheet, 16);

  RenderTexture2D target = LoadRenderTexture(320, 180);
  SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

  Grid *grid1 = new Grid({0, 0}, {1, 1});   // NE
  Grid *grid2 = new Grid({0, 0}, {-1, 1});  // SE
  Grid *grid3 = new Grid({0, 0}, {-1, -1}); // SW
  Grid *grid4 = new Grid({0, 0}, {1, -1});  // NW

  // just going to hard code a loop that displays the recently chopped up sprite
  // sheet to demo the grid it looks like the positioning for the tiles is off
  auto func = [&]() {
    float cellHeight = float(floorSpriteSheet.height) / 16;
    float cellWidth = float(floorSpriteSheet.width) / 16;
    int i = 0;
    for(int w = 0; w * cellWidth < floorSpriteSheet.width; w++)
      {
        for(int h = 0; h * cellHeight < floorSpriteSheet.height; h++)
          {
            if(renderData.size() <= static_cast<size_t>(i))
              return;
            grid1->UpdateTile({h, w}, renderData[i]);
            i++;
          }
      }
  };
  func();

  // auto DrawTick = std::async(std::launch::async, &RenderSystem::Draw,
  // Renderer);
  //  maybe we render async?

  // so on draw tick we fill each slot of render data
  // then we render them
  // keep it simple just render a layer at a time
  // the draw tick gets called on game loop
  //
  // but we redner async as fast as we can by loop

  // for the render system I need to rethink the pipeline so that it works with
  // the texture we throw everything into

  std::span<RenderData> span(renderData);
  float gameScreenWidth = 640;
  float gameScreenHeight = 360;

  while(!WindowShouldClose())
    {
      float scale = std::min((float)GetScreenWidth() / gameScreenWidth,
                             (float)GetScreenHeight() / gameScreenHeight);
      BeginTextureMode(target);
      grid1->DrawTick(span, 0);
      EndTextureMode();
      BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawTexturePro(
        target.texture,
        {0, 0, (float)target.texture.width, (float)-target.texture.height},
        {0, 0, (float)gameScreenWidth * scale,
         (float)gameScreenHeight * scale},
        {0, 0}, 0.0f, WHITE);
      EndDrawing();
    }
  CloseWindow();
  return 0;
}
