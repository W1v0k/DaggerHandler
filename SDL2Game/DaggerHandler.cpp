// SDL2Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SDL.h>
#include "Game.h"

Game* game = nullptr;
int main(int argc, char* argv[])
{

	const int FPS = 60; //refresh rate of monitors
	const int frameDelay = 1000 / FPS;//max time between frames

	Uint32 frameStart; //unsigned int
	int frameTime;

	game = new Game();
	game->init("DaggerHandler", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,512,512,false);
	
	while (game->running())
	{
		frameStart = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart; //how long is taking to go through the events

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}
	game->clean();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
