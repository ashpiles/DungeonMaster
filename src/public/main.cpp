#include "CoreTypes.h"
#include "CoreUtil.h"
#include "WorldGen/Tiles.h"
#include "raylib.h"

/*
 *  Lets just do a mystery dungeon instead.
 *
 * fill the grid with floors
 * change floor color at a range (when 5 tiles away from 0,0 change color ect)
 *
 * create player actor
 * move camera & player actor with wasd
 */

/*
 * to fill the scene with floor tiles I need to fill the grid with tiles
 * I also need to fill the render system with enough tiles to render the screen
 * TODO: get the number of tiles necassasry to fill the screen
 * TODO: create that number of render data objs
 * TODO: create sprite atlas system again
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

  Texture2D floorSpriteSheet = LoadTexture(
      "/home/ash/Workspace/dev/DungeonMaster/content/32rogues/tiles.png");

  SpriteAtlas floorAtlas(&floorSpriteSheet);

  RenderTexture2D target = LoadRenderTexture(320, 180);
  SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

  Grid *grid = new Grid({0, 0});

  float gameScreenWidth = 640;
  float gameScreenHeight = 360;

  Vector2 camera{0, 0};

  while(!WindowShouldClose())
    {
      float scale = std::min((float)GetScreenWidth() / gameScreenWidth,
                             (float)GetScreenHeight() / gameScreenHeight);

      if(IsKeyDown(KEY_W))
        camera.y -= 1.0f;
      else if(IsKeyDown(KEY_A))
        camera.x -= 1.0f;
      else if(IsKeyDown(KEY_D))
        camera.x += 1.0f;
      else if(IsKeyDown(KEY_S))
        camera.y += 1.0f;

      BeginTextureMode(target);
      grid->DrawTick(0);
      // calls the draw function
      EndTextureMode();

      BeginDrawing();
      ClearBackground(RAYWHITE);
      DrawTexturePro(
          target.texture,
          {0, 0, (float)target.texture.width, (float)-target.texture.height},
          {0, 0, (float)gameScreenWidth * scale,
           (float)gameScreenHeight * scale},
          {camera.x * 16, camera.y * 16}, 0.0f, WHITE);
      EndDrawing();
    }
  CloseWindow();
  return 0;
}
