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
	void oldrecordMove();
	void newrecordMove();
	void clearLevelData();
	void validateMap();
	bool levelIsValid() const;
	bool loadLevel(const std::string level);
	void loadRow(const std::string line, const unsigned int row);
	bool loadNextLevel();
	void restartCurrentLevel();	

	void movePlayer(const Direction direction);
	bool move(const unsigned int row, const unsigned int col,
		const Direction direction, const State state);
	bool moveObject(const unsigned int row, const unsigned int col, int rowMovement,
		int colMovement, const State state);


private:
	std::vector<std::vector<Block>> map;
	std::vector<std::string> lines;
	SDL_Rect playerPos;
	int record;

	unsigned int blockCols;
	unsigned int blockRows; 
	int blockWidth; 
	int blockHeight; 
	RenderManager renderManager;
	unsigned int playerCount; 
	unsigned int boxCount;
	unsigned int boxesOnPlace;
	unsigned int marksCount;
	unsigned int moveCount;
	bool playerIsMoving;
	bool firstCheck;
	unsigned int currentLevel;
	unsigned int levelNum;
};
#endif 
