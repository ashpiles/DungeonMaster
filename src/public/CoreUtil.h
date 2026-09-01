#include "raylib.h"
#include <unordered_map>

struct TileCoordinate;
class Grid;
struct RenderData;

class CoreUtil
{
public:
  CoreUtil() = delete;
  ~CoreUtil() = delete;
  static Vector2 GetTrueCoordinates(Grid *grid, const TileCoordinate &coord);

  static Vector2
  GetCenterTrueCoordinates(Grid *grid, const TileCoordinate &coord);

  static std::unordered_map<TileCoordinate, RenderData>
  CreateRenderDataFromSpriteSheet(Texture2D &texture, float cellSize);
};
