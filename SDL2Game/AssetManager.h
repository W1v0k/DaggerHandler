#pragma once

#include "SDL.h"
#include <map> //hash table
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h" //fonts

class AssetManager//creaza texturile si creaza obiectele jocului
{
private:
	std::map<std::string, SDL_Texture*> textures; //cheile vor fi stringuri(id), iar valorile vor fi texturi
	Manager* manager;
	std::map<std::string, TTF_Font*> fonts;
public:
	AssetManager(Manager* man);
	~AssetManager();
	//GameObjects

	void CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id);//positia unde vrem sa creem

	//Texture Management
	void AddTexture(std::string id, const char* path);
	SDL_Texture* GetTexture(std::string id);

	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);
};