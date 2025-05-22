#include <SDL_image.h>
#include "RenderManager.h"
#include "Block.h"

bool RenderManager::InitEverything()
{
	if (!InitSDL())
		return false;
	if (!CreateWindow())
		return false;
	if (!CreateRenderer())
		return false;
	if (!SetupTTF("Roboto_Condensed-Black.ttf"))
		return false;
	SetupRenderer();
	return true;
}
bool RenderManager::InitSDL()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}
bool RenderManager::CreateWindow()
{
	window = SDL_CreateWindow("Sokoban", posX, posY, sizeX, sizeY, 0);

	if (window == nullptr)
	{
		std::cout << "Failed to create window : " << SDL_GetError();
		return false;
	}
	return true;
}
bool RenderManager::CreateRenderer()
{
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == nullptr)
	{
		std::cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}
	return true;
}
void RenderManager::SetupRenderer()
{
	SDL_RenderSetLogicalSize(renderer, sizeX, sizeY);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

bool RenderManager::SetupTTF(const std::string& fontName)
{
	if (TTF_Init() == -1)
	{
		std::cout << " Failed to initialize TTF : " << SDL_GetError() << std::endl;
		return false;
	}
	font = TTF_OpenFont(fontName.c_str(), 90);
	if (font == nullptr)
	{
		std::cout << " Failed to load font : " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

SDL_Texture* RenderManager::SurfaceToTexture(SDL_Surface* surf)
{
	SDL_Texture* text;
	text = SDL_CreateTextureFromSurface(renderer, surf);
	SDL_FreeSurface(surf);
	return text;
}

unsigned int RenderManager::getWindowHeight() const
{
	return sizeY;
}
void RenderManager::Render(std::vector<std::vector<Block>>& map, bool isFirstRender, bool shouldFlip, const std::string& msg, const std::string& rec)
{
	if (isFirstRender)
	{
		playerTexture = LoadTexture(playerImage);
		wallTexture = LoadTexture(wallImage);
		boxTexture = LoadTexture(boxImage);
		markTexture = LoadTexture(markImage);
	}
	SDL_RenderClear(renderer);
	for (std::vector<Block>& currRow : map)
	{
		for (Block& currBlock : currRow)
		{
			if (currBlock.state == WALL)
				SDL_RenderCopy(renderer, wallTexture, NULL, &currBlock.position);
			else if (currBlock.state == BOX)
				SDL_RenderCopy(renderer, boxTexture, NULL, &currBlock.position);
			else if (currBlock.state == PLAYER)
			{
				if (shouldFlip)
				{
					SDL_RendererFlip flipPlayer = SDL_FLIP_HORIZONTAL;
					SDL_RenderCopyEx(renderer, playerTexture, NULL,
						&currBlock.position, 0, NULL, flipPlayer);
				}
				else
					SDL_RenderCopy(renderer, playerTexture, NULL, &currBlock.position);
			}
			else if (currBlock.state == FLOOR && currBlock.onMark)
				SDL_RenderCopy(renderer, markTexture, NULL, &currBlock.position);
			else
				SDL_RenderFillRect(renderer, &currBlock.position);
		}
	}
	font = TTF_OpenFont("Roboto_Condensed-Black.ttf", 10);
	SDL_Surface* blendedText = TTF_RenderText_Blended(font, msg.c_str(), textColor);
	SDL_Texture* textTexture = SurfaceToTexture(blendedText);
	SDL_Rect textRect = { 700, 500, 1, 1 };
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_Surface* newblendedText = TTF_RenderText_Blended(font, rec.c_str(), textColor);
	SDL_Texture* newtextTexture = SurfaceToTexture(newblendedText);
	SDL_Rect recRect = { 700, 550, 1, 1 };
	SDL_QueryTexture(newtextTexture, NULL, NULL, &recRect.w, &recRect.h);
	SDL_RenderCopy(renderer, newtextTexture, NULL, &recRect);
	std::string restartMessage = "Press R to restart";
	SDL_Surface* restartblendedText = TTF_RenderText_Blended(font, restartMessage.c_str(), textColor);
	SDL_Texture* restarttextTexture = SurfaceToTexture(restartblendedText);
	SDL_Rect restartRect = { 500, 600, 1, 1 };
	SDL_QueryTexture(restarttextTexture, NULL, NULL, &restartRect.w, &restartRect.h);
	SDL_RenderCopy(renderer, restarttextTexture, NULL, &restartRect);
	std::string escapeMessage = "Press ESC to exit";
	SDL_Surface* escapeblendedText = TTF_RenderText_Blended(font, escapeMessage.c_str(), textColor);
	SDL_Texture* escapetextTexture = SurfaceToTexture(escapeblendedText);
	SDL_Rect escapeRect = { 500, 650, 1, 1 };
	SDL_QueryTexture(escapetextTexture, NULL, NULL, &escapeRect.w, &escapeRect.h);
	SDL_RenderCopy(renderer, escapetextTexture, NULL, &escapeRect);
	SDL_RenderPresent(renderer);
}
void RenderManager::RenderMoveCount(const std::string& msg) {
	font = TTF_OpenFont("Roboto_Condensed-Black.ttf", 90);
	SDL_Surface* blendedText = TTF_RenderText_Blended(font, msg.c_str(), textColor);
	SDL_Texture* textTexture = SurfaceToTexture(blendedText);
	SDL_Rect textRect = { 800, 600, 1, 1 };
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_RenderPresent(renderer);
}
void RenderManager::RenderMessage(const std::string& msg)
{
	SDL_RenderClear(renderer);
	font = TTF_OpenFont("Roboto_Condensed-Black.ttf", 90);
	SDL_Surface* blendedText = TTF_RenderText_Blended(font, msg.c_str(), textColor);
	SDL_Texture* textTexture = SurfaceToTexture(blendedText);

	SDL_Rect textRect = { sizeX / 4, sizeY / 4, 50, 50 };
	SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
	if (msg == "YOU WON!")
		textRect.x -= 50;
	else
		textRect.x -= 150;
	SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
	SDL_RenderPresent(renderer);
}
unsigned int RenderManager::getWindowWidth() const
{
	return sizeX;
}
SDL_Texture* RenderManager::LoadTexture(const std::string& str)
{
	SDL_Surface* surface = IMG_Load(str.c_str());
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	return texture;
}
