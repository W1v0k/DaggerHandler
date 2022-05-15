#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <vector>
//#include "AssetManager.h" //=>CREA DEPENDENTA CIRCULARA 
#include <iostream>

class AssetManager;
class ColliderComponent;

class Game
{
private:
	int count = 0;
	SDL_Window* window;
public:
	Game();
	~Game();

	void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen);
	void init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen, int level);
	void handleEvents();
	void update();
	void render();
	void clean();
	bool running() { return isRunning; } //inline
	//void updateLevel();

	static bool isRunning;
	static int levelNumber;
	//static void AddTile_v2(int srcX, int srcY, int xpos, int ypos);
	static SDL_Renderer* renderer;
	static SDL_Event event;
	//static std::vector<ColliderComponent*> colliders; //lista de collidere
	static SDL_Rect camera;
	static AssetManager* assets;
	static int playerCoins;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupColliders,
		groupProjectiles
	};
};