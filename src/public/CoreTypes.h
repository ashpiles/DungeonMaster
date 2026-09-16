#pragma once
#include "raylib.h"
#include "stdlib.h"
#include <span>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct RenderData;
struct IntVector;
struct Sprite;

/*===========================================================*/
//			Defines & types
/*===========================================================*/
#define RENDER_STACK_LENGTH 3
#define TILE_SIZE 32

/*===========================================================*/
//			Interfaces
/*===========================================================*/

class ISystem
{
public:
  virtual const ISystem *GetSystem() = 0;
};

class IRender
{
public:
  virtual void DrawTick(std::span<RenderData> &drawStack, float delta) = 0;
};

/*===========================================================*/
//	        		Struct
/*===========================================================*/

struct IntVector
{
  int x = 0;
  int y = 0;

  IntVector operator+(const IntVector &other) const
  { return IntVector{x + other.x, y + other.y}; }
  IntVector operator-(const IntVector &other) const
  { return IntVector{x - other.x, y - other.y}; }
  bool operator==(const IntVector &other)
  { return x == other.x && y == other.y; }
  bool operator==(const IntVector &other) const
  { return x == other.x && y == other.y; }
};

template <> struct std::hash<IntVector>
{
  std::size_t operator()(const IntVector &coord) const
  {
    std::size_t h1 = std::hash<int>{}(coord.x);
    std::size_t h2 = std::hash<int>{}(coord.y);
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
  }
};

// its bad I'm giving it a loose raw pointer I need to handle that
// with some guard rails later
struct Sprite
{
  Texture2D *texture;
  Rectangle frame;
};

struct RenderData
{
private:
  Sprite sprite;
  Sprite **animation;

public:
  RenderData() = default;
  // move constructor
  RenderData(RenderData &&other) noexcept
      : sprite(std::move(other.sprite)),
        animation(std::exchange(other.animation, nullptr)),
        deltaTime(other.deltaTime),
        numOfSkippedCycles(other.numOfSkippedCycles)
  {}
  RenderData(Sprite &&other) : sprite(std::move(other)) {}

  // move assignment
  RenderData &operator=(RenderData &&other)
  {
    if(this != &other)
      {
        sprite = std::move(other.sprite);
        animation = std::exchange(other.animation, nullptr);
        deltaTime = other.deltaTime;
        numOfSkippedCycles = other.numOfSkippedCycles;
        initalizedTruePosition = other.initalizedTruePosition;
      }

    return *this;
  };

  RenderData &operator=(Sprite &&other)
  {
    sprite = std::move(other);
    return *this;
  }

  bool Apply(RenderData &data);
  bool Apply(RenderData &&data);
  bool Apply(RenderData *data);
  bool ApplyInternal(Sprite &&data);
  bool DrawSprite() const;

  float deltaTime;
  int numOfSkippedCycles;
  bool initalizedTruePosition = false;
  Vector2 renderPos;
};

struct SpriteAtlas
{
  SpriteAtlas(Texture2D *sheet);

  const Sprite *const GetSpritePtr(IntVector coord);

private:
  Texture2D *const spriteSheet;
  std::unordered_map<IntVector, Sprite> atlas;
};

/*===========================================================*/
//	        	BaseClasses
/*===========================================================*/

class Actor : public IRender
{
  IntVector coordinate{};
  Texture2D textureResource;

public:
  virtual void
  DrawTick(std::span<RenderData> &drawStack, float delta) override;
};

/*===========================================================*/
//			Singletons
/*===========================================================*/

// instead of a singleton this could be a config file at some point
// with a service that will go and read the file, sort of like how UE
// does configs, but for the sake of prototyping a singleton is perfectly fine
class GlobalSettings : public ISystem
{
public:
  GlobalSettings();
  static GlobalSettings *GetSettings();
  virtual const ISystem *GetSystem();

  int screenWidth;
  int screenHeight;
  int targetFPS;
  std::string windowName;
  float tileSize;

private:
  static GlobalSettings *instance;
};

// This should be turned into a service as well
// the idea is that our RenderSystem will hold the
// memory of our render data in an object pool
// this memory has no context on where to render to
//
// we also assign the

class RenderSystem : public ISystem
{
public:
  RenderSystem();
  static RenderSystem *GetRenderSystem();
  virtual const ISystem *GetSystem();

  void CallDrawTick();
  void Draw();

private:
  static RenderSystem *instance;

  // Objects that must be drawn next tick
  std::vector<IRender *> drawStack;
  // Memory we give to objects
  std::vector<RenderData> renderMemory[RENDER_STACK_LENGTH];
};
