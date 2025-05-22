#ifndef __RENDER__MANAGER__HEADER__INCLUDED
#define __RENDER__MANAGER__HEADER__INCLUDED

#include <iostream>
#include <vector>
#include <SDL_ttf.h>
#include "Assets.h"
#include "Block.h"
class RenderManager
{
public:
	RenderManager() { InitEverything(); };
	~RenderManager() = default;

	void Render(std::vector<std::vector<Block>>& map, bool isFirstRender, bool shouldFlip, const std::string& msg,const std::string& rec);
	void RenderMoveCount(const std::string& msg);
	void RenderMessage(const std::string& msg);

	unsigned int getWindowWidth() const;
	unsigned int getWindowHeight() const;

private:
	bool InitEverything();
	bool InitSDL();
	bool CreateWindow();
	bool CreateRenderer();
	void SetupRenderer();
	bool SetupTTF(const std::string& fontName);

	SDL_Texture* LoadTexture(const std::string& str);
	SDL_Texture* SurfaceToTexture(SDL_Surface* surf);

	int posX = 300; 
	int posY = 150;
	int sizeX = 1200;
	int sizeY = 800;

	SDL_Rect backgroundPos{ 0, 0, sizeX, sizeY };
	SDL_Texture* playerTexture;
	SDL_Texture* wallTexture;
	SDL_Texture* boxTexture;
	SDL_Texture* markTexture;

	SDL_Window* window;
	SDL_Renderer* renderer;

	TTF_Font* font;
	SDL_Color textColor = { 255, 255, 255, 255 }; 
};
#endif
