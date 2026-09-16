#pragma once
#include "raylib.h"

struct IntVector;
class Grid;

class CoreUtil
{
public:
  CoreUtil() = delete;
  ~CoreUtil() = delete;
  static Vector2 GetTrueCoordinates(Grid *grid, const IntVector &coord);

  static Vector2 GetCenterTrueCoordinates(Grid *grid, const IntVector &coord);
};
