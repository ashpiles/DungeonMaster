#include "raylib.h"
#include <vector>

struct TileCoordinate;
class Grid;
struct RenderData;

class CoreUtil {
public:
  CoreUtil() = delete;
  ~CoreUtil() = delete;
  static Vector2 GetTrueCoordinates(Grid *grid, TileCoordinate &coord);

  static Vector2 GetCenterTrueCoordinates(Grid *grid, TileCoordinate &coord);

  static std::vector<RenderData>
  CreateRenderDataFromSpriteSheet(Texture2D &texture, float cellSize);
};
