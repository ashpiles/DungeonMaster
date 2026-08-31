#include "raylib.h"
#include "CoreTypes.h"


namespace core
{
	static Vector2 GetTrueCoordinates(TileCoordinate* coord)
	{
		float tileSize = GlobalSettings::GetSettings()->tileSize;
		return {coord->X * tileSize, coord->Y * tileSize};
	};

	static Vector2 GetCenterTrueCoordinates(Grid* grid, TileCoordinate* coord)
	{
		float tileSize = GlobalSettings::GetSettings()->tileSize;
		Vector2 trueCoords = GetTrueCoordinates(coord);
		Vector2 halfExtents = {grid->direction.X * (tileSize/2), grid->direction.Y * (tileSize/2)};
		return {trueCoords.x + halfExtents.x, trueCoords.y + halfExtents.y};
	};

	static std::vector<RenderData> CreateRenderDataFromSpriteSheet(Texture2D& texture, float cellSize)
	{
		std::vector<RenderData> data;
		float cellHeight = texture.height / cellSize;
		float cellWidth = texture.width / cellSize;
		for (int h = cellHeight; h < texture.height; h += cellHeight)
		{
			for (int w = cellWidth; w < texture.width; w += cellWidth)
			{
				//data.emplace()
			}
		}
		// for
	};
}
