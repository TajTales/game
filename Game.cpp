#include "Game.h"

Game::Game()
	:blockWidth(width)
	, blockHeight(height)
	, marksCount(0)
	, boxCount(0)
	, boxesOnPlace(0)
	, playerCount(0)
	, playerIsMoving(false)
	, currentLevel(0)
	, levelNum(10)
{
	if (loadNextLevel())
		renderManager.Render(map, true, false);
	else
	{
		renderManager.RenderMessage("No valid levels.");
		SDL_Delay(2000);
		std::exit(-1);
	}
}

void Game::runGame()
{
	bool loop = true;
	bool isFirst = true;
	bool shouldFlip = false;

	while (loop)
	{
		handleInput(loop, shouldFlip);
		renderManager.Render(map, isFirst, shouldFlip);
		isFirst = false;
		SDL_Delay(30); 
		update(loop);
	}
}

bool Game::loadLevel(const std::string level)
{
	std::ifstream file(level);
	if (!file.is_open())
	{
		std::cerr << "Error when loading level. The program will quit.\n";
		return false;
	}
	map.push_back(std::vector<Block>());
	playerPos.h = blockHeight;
	playerPos.w = blockWidth;

	std::string current;
	std::getline(file, current, '\n');

	blockCols = current.size();
	blockRows = 0;

	loadRow(current, blockRows++);
	while (!file.eof())
	{
		map.push_back(std::vector<Block>());
		std::getline(file, current, '\n');
		loadRow(current, blockRows++);
	}
	file.close();
	return levelIsValid();
}

void Game::handleInput(bool& loop, bool& shouldFlip)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (playerIsMoving)
			continue;
		if (event.type == SDL_QUIT)
			loop = false;
		else if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_RIGHT:
				movePlayer(RIGHT);
				shouldFlip = true;
				break;
			case SDLK_LEFT:
				movePlayer(LEFT);
				shouldFlip = false;
				break;
			case SDLK_UP:
				movePlayer(UP);
				break;
			case SDLK_DOWN:
				movePlayer(DOWN);
				break;
			default:
				break;
			}
		}
	}
}
void Game::update(bool& loop)
{
	if (boxCount == boxesOnPlace)
	{
		std::string strLevel = "Level ";
		strLevel += std::to_string(currentLevel);
		strLevel.append(" completed");
		renderManager.RenderMessage(strLevel);
		SDL_Delay(2000);

		if (!loadNextLevel())
		{
			renderManager.RenderMessage("YOU WON!");
			SDL_Delay(2000);
			loop = false;
		}
	}
}
void Game::clearLevelData()
{
	if (!map.empty())
	{
		for (int i = blockRows - 1; i >= 0; --i)
		{
			for (unsigned int j = map[i].size(); j > 0; --j)
				map[i].pop_back();
			map.pop_back();
		}
	}
	marksCount = 0;
	boxCount = 0;
	boxesOnPlace = 0;
	playerCount = 0;
	playerIsMoving = false;
}

void Game::validateMap()
{
	for (unsigned int i = 0; i < blockRows; ++i)
	{
		for (unsigned int j = map[i].size(); j < blockCols; ++j)
			map[i].push_back(Block(i, j, FLOOR, false));
	}
}
void Game::loadRow(const std::string line, const unsigned int row)
{
	State currState;
	bool onGoal = false;
	unsigned int len = line.size();
	if (len > blockCols)
		blockCols = len;
	for (unsigned int i = 0; i < len; ++i)
	{
		switch (line[i])
		{
		case '#':
			currState = WALL;
			break;
		case '@':
			currState = PLAYER;
			playerPos.x = blockWidth * i;
			playerPos.y = blockHeight * row;
			++playerCount;
			break;
		case '+':
			currState = PLAYER;
			onGoal = true;
			playerPos.x = blockWidth * i;
			playerPos.y = blockHeight * row;
			++playerCount;
			++marksCount;
			break;
		case '$':
			currState = BOX;
			++boxCount;
			break;
		case '*':
			currState = BOX;
			onGoal = true;
			++boxCount;
			++boxesOnPlace;
			++marksCount;
			break;
		case '.':
			currState = FLOOR;
			onGoal = true;
			++marksCount;
			break;
		case ' ':
			currState = FLOOR;
			break;
		default:
			currState = WALL;
			break;
		}
		map[row].push_back(Block(row, i, currState, onGoal));
		onGoal = false;
	}
}
bool Game::loadNextLevel()
{
	clearLevelData();
	std::string strLevel = "Levels\\level";
	strLevel += std::to_string(++currentLevel);
	strLevel += ".txt";
	while (currentLevel <= levelNum && !loadLevel(strLevel))
	{
		std::string message = "Level ";
		message += std::to_string(currentLevel);
		message += " is invalid.";
		renderManager.RenderMessage(message);
		SDL_Delay(2000);
		clearLevelData();
		strLevel = "level";
		strLevel += std::to_string(++currentLevel);
		strLevel += ".txt";
	}
	if (currentLevel <= levelNum)
	{
		validateMap();
		return true;
	}
	return false;
}

void Game::movePlayer(Direction direction)
{
	unsigned int row = playerPos.y / blockWidth;
	unsigned int col = playerPos.x / blockHeight;
	move(row, col, direction, PLAYER);
}

bool Game::move(const unsigned int row, const unsigned int col,
	const Direction direction, const State state)
{
	if (direction == UP)
		moveObject(row, col, -1, 0, state);
	else if (direction == DOWN)
		moveObject(row, col, 1, 0, state);
	else if (direction == LEFT)
		moveObject(row, col, 0, -1, state);
	else if (direction == RIGHT)
		moveObject(row, col, 0, 1, state);
	return true;
}
bool Game::moveObject(const unsigned int row, const unsigned int col, int rowMovement, int colMovement, const State state)
{
	if ((map[row + rowMovement][col + colMovement].state == WALL) ||
		(state == BOX && map[row + rowMovement][col + colMovement].state == BOX))
		return false;
	if (map[row + rowMovement][col + colMovement].state == BOX)
	{
		if (moveObject(row + rowMovement, col + colMovement, rowMovement, colMovement, BOX))
			map[row + (rowMovement * 2)][col + (colMovement * 2)].state = BOX;
		else
			return false;
	}
	if (state == PLAYER)
	{
		map[row + rowMovement][col + colMovement].state = PLAYER;
		playerPos.y += (rowMovement * blockHeight);
		playerPos.x += (colMovement * blockWidth);
		map[row][col].state = FLOOR;
	}
	else if (state == BOX) 
	{
		if (map[row][col].onMark)
			--boxesOnPlace;
		if (map[row + rowMovement][col + colMovement].onMark)
			++boxesOnPlace;
	}
	return true;
}
bool Game::levelIsValid() const
{
	if (boxCount > marksCount || !boxCount || playerCount != 1)
		return false;
	return true;
}
