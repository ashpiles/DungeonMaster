#include "raylib.h"
#include "CoreTypes.h"
#include "CoreUtil.h"
#include <iostream>

/*
* 1 minute bullet chess,
* but you unlock pieces types like a tier tree
*
* its like tower defense; someone else can play the humans
*
*
* fly weight for the tile assets
* // config file somewhere which outlines what goes to what
* // 
*/



int main(void)
{

	GlobalSettings* Settings = GlobalSettings::GetSettings();
	RenderSystem* Renderer = RenderSystem::GetRenderSystem();
	Settings->screenHeight = 450;
	Settings->screenWidth = 800;
	Settings->windowName = "DungeonMaster_V0.0.1";
	Settings->targetFPS = 60;
	Settings->tileSize = 16;

	InitWindow(Settings->screenWidth, Settings->screenHeight, Settings->windowName.c_str());
	Texture2D floorSpriteSheet = LoadTexture("/home/ash/Workspace/dev/DungeonMaster/content/Tiled_files/walls_floor.png");
	std::vector<RenderData> renderData = CoreUtil::CreateRenderDataFromSpriteSheet(floorSpriteSheet, 16);

	RenderTexture2D target = LoadRenderTexture(320, 180);
	SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);

	Grid* grid1 = new Grid({0,0}, {1,1});	//NE
	Grid* grid2 = new Grid({0,0}, {-1,1});	//SE
	Grid* grid3 = new Grid({0,0}, {-1,-1}); //SW
	Grid* grid4 = new Grid({0,0}, {1,-1});	//NW

	// everything sort of works but we setting every piece of render data to the same thing
	// also we are not expanding to the grid size we want
	// + expanding grid size with update tile is a bit clunky and confusing so I think that should be removed
	//grid1->UpdateTile({0,0}, renderData[38], true);

	// just going to hard code a loop that displays the recently chopped up sprite sheet to demo the grid
	auto func = [&]()
	{
		float cellHeight = float(floorSpriteSheet.height) / 16;
		float cellWidth = float(floorSpriteSheet.width) / 16;
		int i = 0;
		for (int h = 0; h * cellHeight < floorSpriteSheet.height; h++)
		{	
			for (int w = 0; w * cellWidth < floorSpriteSheet.width; w++)
			{
				if (renderData.size() <= static_cast<size_t>(i))
					return;
				grid1->UpdateTile({h,w}, renderData[i], true);
				i++;
			}
		}
	};
	func();

	//auto DrawTick = std::async(std::launch::async, &RenderSystem::Draw, Renderer);
	// maybe we render async?

// so on draw tick we fill each slot of render data
// then we render them
// keep it simple just render a layer at a time
// the draw tick gets called on game loop
//
// but we redner async as fast as we can by loop


	std::span<RenderData> span(renderData);
	float gameScreenWidth = 320;
	float gameScreenHeight = 180;
	
	while (!WindowShouldClose()) 
	{
		float scale = std::min((float)GetScreenWidth()/gameScreenWidth, (float)GetScreenHeight()/gameScreenHeight);
		BeginTextureMode(target);
			grid1->DrawTick(span, 0);
		EndTextureMode();
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawTexturePro(target.texture,
				(Rectangle){0,0, (float)target.texture.width,(float)-target.texture.height},
				(Rectangle){GetScreenWidth() - ((float)gameScreenWidth * scale)* 0.5f, GetScreenHeight() - ((float)gameScreenHeight*scale)*0.5f,
					(float)gameScreenWidth*scale, (float)gameScreenHeight*scale},
				Vector2{0,0},
				0.0f,
				WHITE);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

