#pragma once

#include "ECS.h"
//#include "TransformComponent.h"
//#include "SpriteComponent.h"
#include "SDL.h"
#include "Vector2D.h"
#include "Game.h"
#include "TextureManager.h"

//template<typename... Args>
//std::string string_format(const char* fmt, Args... args)
//{
//	size_t size = snprintf(nullptr, 0, fmt, args...);
//	std::string buf;
//	buf.reserve(size + 1);
//	buf.resize(size);
//	snprintf(&buf[0], size + 1, fmt, args...);
//	return buf;
//}

class TileComponent : public Component
{
public:
	//TransformComponent* transform;
	//SpriteComponent* sprite;

	//SDL_Rect tileRect;
	//int tileID;
	//char* path;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;//pentru a tine cont unde sunt tile-urile cu adevarat, nu unde sunt de fapt desenate

	TileComponent() = default; //default constructor
	
	~TileComponent() { SDL_DestroyTexture(texture); }
	
	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id)
	{
		texture = Game::assets->GetTexture(id);
		
		position.x = static_cast<float>(xpos);
		position.y = static_cast<float>(ypos);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;

		//destRect.x = xpos;
		//destRect.y = ypos;
		destRect.w = destRect.h = tsize * tscale;
	}

	void update() override
	{
		destRect.x = position.x - Game::camera.x;
		destRect.y = position.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	/*void init() override
	{
		entity->addComponent<TransformComponent>((float)tileRect.x, (float)tileRect.y, tileRect.w, tileRect.h, 1);
		transform = &entity->getComponent<TransformComponent>();
		
		entity->addComponent<SpriteComponent>(path);
		sprite = &entity->getComponent<SpriteComponent>();
	}*/

	//TileComponent(int x, int y, int w, int h, int id)
	//{	
	//	tileRect.x = x;
	//	tileRect.y = y;
	//	tileRect.w = w;
	//	tileRect.h = h;
	//	tileID = id;

	//if (tileID <= 56)
	//{
	//	if (tileID <= 9)
	//	{
	//		std::string buffer = string_format("assets/tiles2/tile00%d.png", tileID);
	//		path = new char[buffer.length()];
	//		strcpy_s(path, buffer.length() + 1, buffer.c_str()); //c_str() creeaza un string de C adica un sir de caractere
	//	}
	//	else
	//	{
	//		std::string buffer = string_format("assets/tiles2/tile0%d.png", tileID);
	//		path = new char[buffer.length()];
	//		strcpy_s(path, buffer.length() + 1, buffer.c_str());
	//	}
	//}
	//if (tileID >= 57 && tileID <= 74)
	//{
	//	std::string buffer = string_format("assets/tiles2/tile0%d.png", tileID);
	//	path = new char[buffer.length()];
	//	strcpy_s(path, buffer.length() + 1, buffer.c_str());
	//}
	//
	//if (tileID >= 75 && tileID <= 142)
	//{
	//	if (tileID <= 99)
	//	{
	//		std::string buffer = string_format("assets/tiles2/tile0%d.png", tileID);
	//		path = new char[buffer.length()];
	//		strcpy_s(path, buffer.length() + 1, buffer.c_str());
	//	}
	//	else
	//	{
	//		std::string buffer = string_format("assets/tiles2/tile%d.png", tileID);
	//		path = new char[buffer.length()];
	//		strcpy_s(path, buffer.length() + 1, buffer.c_str());
	//	}
	//}
	//}
	
};
