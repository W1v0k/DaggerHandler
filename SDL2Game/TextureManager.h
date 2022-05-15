#pragma once
#include "Game.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* fileName);//calling it as a class func, rather than a member func belonging to an obj
	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip); //se poate selecta tot randul asta, clicl dreapta, Quick Actions, create Declaration/Definition
};