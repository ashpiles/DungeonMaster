#include "raylib.h"
#include "CoreTypes.h"
#include <future>

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

	InitWindow(Settings->screenWidth, Settings->screenHeight, Settings->windowName.c_str());
	RenderData renderData({LoadTexture("/home/ash/Workspace/dev/DungeonMaster/content/Tiled_files/walls_floor.png")});

	// need to cut the render data into the frames

	Grid* grid1 = new Grid({0,0}, {1,1});	//NE
	Grid* grid2 = new Grid({0,0}, {-1,1});	//SE
	Grid* grid3 = new Grid({0,0}, {-1,-1}); //SW
	Grid* grid4 = new Grid({0,0}, {1,-1});	//NW

	grid1->UpdateTile({0,0}, renderData, true);

	//auto DrawTick = std::async(std::launch::async, &RenderSystem::Draw, Renderer);
	// maybe we render async?

// so on draw tick we fill each slot of render data
// then we render them
// keep it simple just render a layer at a time
// the draw tick gets called on game loop
//
// but we redner async as fast as we can by loop


	while (!WindowShouldClose()) 
	{
		BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawText("Text", 190, 200, 20, LIGHTGRAY);
		EndDrawing();
	}
	CloseWindow();
	return 0;
}

