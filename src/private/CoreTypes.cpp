#include "CoreTypes.h"
#include "CoreUtil.h"
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
	if(!mutex.try_lock())
		return false;
	sprite = std::move(data);
	mutex.unlock();
	return true;
}

bool RenderData::Apply(RenderData& data)
{
	if(!mutex.try_lock())
		return false;
	sprite = data.sprite;
	return true;
}

bool RenderData::Apply(Vector2&& pos)
{
	sprite.position = pos;
	return true;
}

bool RenderData::DrawSprite()
{
	if(!mutex.try_lock())
		return false;
	DrawTextureRec(sprite.texture, sprite.frame, sprite.position, WHITE);
	mutex.unlock();
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
	grid.emplace_back(std::vector<RenderData>(1));
}
Grid::Grid(TileCoordinate&& inOrigin, IntVector&& inDirection) : origin(inOrigin), direction(inDirection)
{

	grid.emplace_back(std::vector<RenderData>(1));
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

void Grid::DrawTick(std::span<RenderData>& drawStack, float delta)
{
	for(int i = 0; i < grid.size(); i++)
	{
		for(int j = 0; j < grid[i].size(); j++)
		{
			TileCoordinate coord {i,j};
			grid[i][j].Apply(CoreUtil::GetCenterTrueCoordinates(this, coord));
			grid[i][j].DrawSprite();
		}
	}
}



// need to write some functions that apply int vectors to tile coordinates
// this will explicitly rely when we are working on a tile position and when we are working with a force
//
//
// maybe the grid is like a crawler and the data underneath is a server??
// that could be cool

bool Grid::UpdateTile(TileCoordinate coord, RenderData& in, bool expandGrid)
{
    if (expandGrid && !WithinGrid(coord))
    {
        ExpandGridTo(coord);
    }

    if (!WithinGrid(coord))
        return false;

    // Now we know the cell exists
    TileCoordinate local = coord - origin;
    local.X = std::abs(local.X);
    local.Y = std::abs(local.Y);
    grid[local.X][local.Y].Apply(in);  // or use move assignment
    return true;
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

    if (grid.size() <= static_cast<size_t>(localCoord.X))
        return false;

    if (grid[localCoord.X].size() <= static_cast<size_t>(localCoord.Y))
        return false;

    return true;
}

bool Grid::ExpandGridTo(TileCoordinate coord)
{
    TileCoordinate local = coord - origin;
    local.X = std::abs(local.X);
    local.Y = std::abs(local.Y);

    size_t neededRows = static_cast<size_t>(local.X) + 1;
    if (grid.size() < neededRows)
        grid.resize(neededRows);

    size_t neededCols = static_cast<size_t>(local.Y) + 1;
    for (auto& row : grid)
    {
        if (row.size() < neededCols)
            row.resize(neededCols);
    }
    return true;
}
