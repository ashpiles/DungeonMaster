#include "raylib.h"
#include "stdlib.h"
#include <string>
#include <span>
#include <utility>
#include <vector>
#include <mutex>

struct RenderData;

/*===========================================================*/
//							Defines	
/*===========================================================*/
#define RENDER_STACK_LENGTH 32

/*===========================================================*/
//							Interfaces	
/*===========================================================*/

class ISystem
{
public:
	virtual const ISystem* GetSystem() = 0;
};

class IBaseRender
{
public:
	virtual void DrawTick_Internal(std::span<RenderData>& stack) = 0;
};

class IRender : public IBaseRender
{
public:
	// we'll treat the array as the z order as well
	virtual void DrawTick(std::span<RenderData>& drawStack, float delta) = 0;

	/*
	 * first phase: async gather the textures for a layer
	 *	-> do some slight continuation passing shennigans
	 *	maybe use async???
	 *
	 * -> fill the memory that we will need
	 * -> go to next obj until out of renderable objs
	 * -> iterate over the draw stack
	 *  -> i could try and pause
	 *
	 *  we call an obj and give it data to fill
	 *  we wrap that in an internal call
	 *  inside the internal call we get the memory to fill
	 *  we could split the filling memory and rendering with async!
	 *
	 *  we need to have meta data about updates
	 */

private:
	virtual void DrawTick_Internal(std::span<RenderData>& stack) override
	{
		DrawTick(stack, 0);
	};
};

class IMultiRender : public IBaseRender
{
public:
	virtual void DrawTick(std::span<std::span<RenderData>>& drawStack, float delta) = 0;

private:
	virtual void DrawTick_Internal(std::span<RenderData>& stack) override
	{
		//DrawTick(stack, 0);
	};
};


/*===========================================================*/
//							Struct	
/*===========================================================*/

struct IntVector
{
	int X = 0;
	int Y = 0;
};

struct TileCoordinate : public IntVector
{
	TileCoordinate operator+(const IntVector& V)
	{
		return TileCoordinate{X + V.X, Y + V.Y};
	}
	TileCoordinate operator-(const IntVector& V)
	{
		return TileCoordinate{X - V.X, Y - V.Y};
	}
};


// this is what we will eventually turn into a flyweight
struct RenderData
{
private:
	struct Internal
	{
		Texture2D texture;
		Rectangle frame;
		Vector2 position;
	};
	Internal sprite;
	Internal** animation;
public:
	RenderData() = default;
	// move constructor 
	RenderData(RenderData&& other) noexcept
	: sprite(std::move(other.sprite)),
	animation(std::exchange(other.animation, nullptr)),
	deltaTime(other.deltaTime),
	numOfSkippedCycles(other.numOfSkippedCycles)
	{}
	RenderData(Internal&& other)
	: sprite(std::move(other)){}



	// move assignment
	RenderData& operator=(RenderData&& other)
	{
		if (this != &other && guard.try_lock())
		{
			sprite = std::move(other.sprite);
			animation = std::exchange(other.animation, nullptr);
			deltaTime = other.deltaTime;
			numOfSkippedCycles = other.numOfSkippedCycles;
			guard.unlock();
		}

		return *this;
	};

	RenderData& operator=(Internal&& other)
	{
		if (guard.try_lock())
		{
			sprite = std::move(other);
		}
		return *this;
	}

	RenderData(const RenderData&) = delete;
	RenderData& operator=(const RenderData&) = delete;
	bool Apply(Internal&& data);
	bool Apply(RenderData& data);
	bool DrawSprite();

	float deltaTime;
	int numOfSkippedCycles;
	std::mutex guard;
};



/*===========================================================*/
//							BaseClasses	
/*===========================================================*/

/*
 * it could be nice to flesh out the grid obj
 * make it explicitly handle tile coordinates
 * maybe write some functions for checking the grid size?
 * -> could turn that into some test functions maybeee
 */

class Grid : public IMultiRender
{
public:
	const TileCoordinate origin;
	const IntVector direction;

	Grid(TileCoordinate& origin, IntVector& direction);
	Grid(TileCoordinate&& origin, IntVector&& direction);
	virtual ~Grid();

	bool GetTileRenderData(TileCoordinate coord, RenderData& out);
	bool UpdateTile(TileCoordinate coord, RenderData& in, bool expandGird = false);
	bool UpdateTiles(TileCoordinate from, TileCoordinate to, RenderData& in, bool expandGrid = false);
	virtual void DrawTick(std::span<std::span<RenderData>>&, float delta) override;

protected:
	bool WithinGrid(TileCoordinate coord);
	bool ExpandGridTo(TileCoordinate coord);

private:
	std::vector<std::vector<RenderData>> grid;
};


class Actor : public IRender
{
	TileCoordinate coordinate{};
	Texture2D textureResource;
public:
	virtual void DrawTick(std::span<RenderData>& drawStack, float delta) override;
};



/*===========================================================*/
//							Singletons	
/*===========================================================*/

class GlobalSettings : public ISystem
{
public:
	GlobalSettings();
	static GlobalSettings* GetSettings();
	virtual const ISystem* GetSystem();

	int screenWidth;
	int screenHeight;
	int targetFPS;
	std::string windowName;
	float tileSize;

private:
	static GlobalSettings* instance;
};



class RenderSystem : public ISystem
{
public:
	RenderSystem();
	static RenderSystem* GetRenderSystem();
	virtual const ISystem* GetSystem();

	void CallDrawTick();
	void Draw();

private:
	static RenderSystem* instance;

	std::vector<IBaseRender*> renderObjs;
	std::vector<RenderData>	renderStack[RENDER_STACK_LENGTH];

};
