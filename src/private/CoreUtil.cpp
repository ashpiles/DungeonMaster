#include "CoreUtil.h"
#include "CoreTypes.h"

Vector2 CoreUtil::GetTrueCoordinates(Grid* grid, TileCoordinate& coord)
{
    float tileSize = GlobalSettings::GetSettings()->tileSize;

    TileCoordinate local = coord - grid->origin;
    return {

        grid->origin.X * tileSize + (grid->direction.X * local.X * tileSize),
        grid->origin.Y * tileSize + (grid->direction.Y * local.Y * tileSize)
    };
}

Vector2 CoreUtil::GetCenterTrueCoordinates(Grid* grid, TileCoordinate& coord)
{
    float tileSize = GlobalSettings::GetSettings()->tileSize;
    Vector2 topLeft = GetTrueCoordinates(grid, coord); // Pass the grid!
    Vector2 halfExtents = {
        grid->direction.X * (tileSize / 2.0f),
        grid->direction.Y * (tileSize / 2.0f)
    };
    return { topLeft.x + halfExtents.x, topLeft.y + halfExtents.y };
}
std::vector<RenderData> CoreUtil::CreateRenderDataFromSpriteSheet(Texture2D &texture, float cellSize)
{
	std::vector<RenderData> data;
	float cellHeight = texture.height / cellSize;
	float cellWidth = texture.width / cellSize;
	RenderData holder({texture});
	data.reserve(int(cellHeight * cellWidth));
	for (int h = cellHeight; h < texture.height; h += cellHeight)
	{	
		for (int w = cellWidth; w < texture.width; w += cellWidth)
		{
			data.emplace_back(RenderData({texture, {float(w), float(h), cellWidth, cellHeight}}));
		}
	}
	return data;
};


