#include "CoreTypes.h"
#include <raylib.h>
#include <vector>



/*===========================================================*/
//						Global Settings
/*===========================================================*/
GlobalSettings* GlobalSettings::instance = nullptr;

GlobalSettings::GlobalSettings()
{
	
}


const ISystem* GlobalSettings::GetSystem()
{
	return GlobalSettings::GetSettings();
}

GlobalSettings* GlobalSettings::GetSettings()
{
	if (!GlobalSettings::instance)
	{
		GlobalSettings::instance = new GlobalSettings;
	}
	return GlobalSettings::instance;
}

/*===========================================================*/
//						Render Data
/*===========================================================*/


bool RenderData::Apply(RenderData::Internal&& data)
{
	if(!guard.try_lock())
		return false;
	sprite = std::move(data);
	guard.unlock();
	return true;
}

bool RenderData::Apply(RenderData& data)
{
	if(!guard.try_lock())
		return false;
	sprite = data.sprite;
	return true;
}

bool RenderData::DrawSprite()
{
	if(!guard.try_lock())
		return false;
	DrawTextureRec(sprite.texture, sprite.frame, sprite.position, WHITE);
	guard.unlock();
	return true;
}




/*===========================================================*/
//						Render System
/*===========================================================*/

RenderSystem* RenderSystem::instance = nullptr;

RenderSystem::RenderSystem(){}

RenderSystem* RenderSystem::GetRenderSystem()
{
	if (!RenderSystem::instance)
	{
		RenderSystem::instance = new RenderSystem;
	}
	return RenderSystem::instance;
}

const ISystem* RenderSystem::GetSystem()
{
	return RenderSystem::GetRenderSystem();
}

void RenderSystem::CallDrawTick()
{
	// we could just put the filling the memory as its own thread
	// i would likley benefit from also putting the drawing loop on that thread to for better animation coordination
	// but for now i'll keep it easier
	for (int layer = 0; layer < RENDER_STACK_LENGTH; layer++)
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
		for (int layer = 0; layer < RENDER_STACK_LENGTH; layer++)
		{
			for (RenderData& data : renderStack[layer])
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
			mean[layer].numOfSkippedCycles = int(mean[layer].numOfSkippedCycles / RENDER_STACK_LENGTH);
		}
		// find the next layer to work on
		for (auto data : mean)
		{}
		// for each tile
		// we check its mutex
		// and update the meta data/ internal data accordingly
//	we also keep track of the cycles meta data avrg per layer
//	so we have data to base what layer needs an update next
	}
};

/*===========================================================*/
//							Grid	
/*===========================================================*/

Grid::Grid(TileCoordinate& inOrigin, IntVector& inDirection) : origin(inOrigin), direction(inDirection)
{
	grid.emplace_back(std::vector<RenderData>{});
}
Grid::Grid(TileCoordinate&& inOrigin, IntVector&& inDirection) : origin(inOrigin), direction(inDirection)
{

	grid.emplace_back(std::vector<RenderData>{});
}

Grid::~Grid()
{

}

bool Grid::GetTileRenderData(TileCoordinate coord, RenderData& out)
{
	if(!WithinGrid(coord))
		return false;

	return true;
}

void Grid::DrawTick(std::span<std::span<RenderData>>& drawStack, float delta)
{}



// need to write some functions that apply int vectors to tile coordinates
// this will explicitly rely when we are working on a tile position and when we are working with a force
//
//
// maybe the grid is like a crawler and the data underneath is a server??
// that could be cool

bool Grid::UpdateTile(TileCoordinate coord, RenderData& in, bool expandGrid)
{
	if (expandGrid)
	{
		if (!WithinGrid(coord))
		{
			if(coord.X == 0 && coord.Y == 0)
				ExpandGridTo({1,1});
		}
		grid[coord.X][coord.Y].Apply(in);
		return true;
	}
	else
	{
		if (!WithinGrid(coord))
			return false;
	}
	return false;
}

bool Grid::UpdateTiles(TileCoordinate from, TileCoordinate to, RenderData& in, bool expandGrid)
{
	return true;
}

bool Grid::WithinGrid(TileCoordinate coord)
{
	TileCoordinate localCoord = coord - origin;
	localCoord.X = std::abs(localCoord.X);
	localCoord.Y = std::abs(localCoord.Y);

	if(grid.size() == 0 || grid.size() < localCoord.X)
		return false;

	if(grid[localCoord.X].size() == 0 || grid[localCoord.X].size() < localCoord.Y)
		return false;
	
	return true;
}

bool Grid::ExpandGridTo(TileCoordinate coord)
{
	if(grid.size() == 0 || (coord.X == 0 && coord.Y == 0))
	{
		return false;
	}
	TileCoordinate localCoord = coord - origin;
	localCoord.X = std::abs(localCoord.X);
	localCoord.Y = std::abs(localCoord.Y);

	if(grid.size() < localCoord.X)
	{
		grid.resize(localCoord.X);
	}
	for(int i = 0; i < grid.size(); i++)
	{
		grid[i].resize(localCoord.Y);
	}
	
	return true;
}
