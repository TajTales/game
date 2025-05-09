#ifndef __GAME__HEADER__INCLUDED
#define __GAME__HEADER__INCLUDED

#include <string>
#include <fstream>
#include <queue> 
#include <set> 
#include <map> 
#include <utility> 
#include "RenderManager.h"
#include "Block.h"

using vertex = std::pair<int, int>;

enum Direction
{
	INVALID_DIR = -1,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Game
{
public:
	Game();
	~Game() = default;

	void runGame();

private:
	void handleInput(bool& loop, bool& shouldFlip);
	void update(bool& loop);

	void clearLevelData();
	void validateMap();
	bool levelIsValid() const;
	bool loadLevel(const std::string level);
	void loadRow(const std::string line, const unsigned int row);
	bool loadNextLevel();

	void movePlayer(const Direction direction);
	bool move(const unsigned int row, const unsigned int col,
		const Direction direction, const State state);
	bool moveObject(const unsigned int row, const unsigned int col, int rowMovement,
		int colMovement, const State state);
private:
	std::vector<std::vector<Block>> map;
	SDL_Rect playerPos;

	unsigned int blockCols;
	unsigned int blockRows; 
	int blockWidth; 
	int blockHeight; 
	RenderManager renderManager;
	unsigned int playerCount; 
	unsigned int boxCount;
	unsigned int boxesOnPlace;
	unsigned int marksCount;
	bool playerIsMoving;

	unsigned int currentLevel;
	unsigned int levelNum;
};
#endif 
