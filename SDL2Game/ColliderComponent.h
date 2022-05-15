#pragma once
#include <string>
#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
//AABB collision - Axis Alligned Bounding Box (se poate folosi direct SDL_IntersectRect)
class ColliderComponent : public Component
{
public:
	SDL_Rect collider; //zona in care vom face coliziune
	std::string tag; //vom stii cu ce facem coliziune

	SDL_Texture* tex;
	SDL_Rect srcR, destR;

	TransformComponent* transform;

	int level;

	ColliderComponent() {}

	ColliderComponent(std::string t)
	{
		tag = t;
	}

	ColliderComponent(std::string t, int xpos, int ypos, int size)
	{
		tag = t;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
		if (tag == "coin")
		{
			tex = TextureManager::LoadTexture("assets/coin.png");
		}
	}

	ColliderComponent(std::string t, char l, int xpos, int ypos, int size)
	{
		tag = t;
		level = atoi(&l);
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
		if (tag == "coin")
		{
			tex = TextureManager::LoadTexture("assets/coin.png");
		}
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>()) // verificam mai intai daca avem componenta de transformare pe entitate
		{
			entity->addComponent<TransformComponent>(); //daca nu, o creem
		}
		transform = &entity->getComponent<TransformComponent>();

		//tex = TextureManager::LoadTexture("assets/ColTex.png");
		srcR = { 0,0, 16, 16 };
		destR = { collider.x, collider.y, collider.w, collider.h };

		//Game::colliders.push_back(this); //creem un collider, il punem in lista
	}

	void update() override
	{
		if (tag != "terrain" && tag != "levelChange" && tag != "coin")
		{
			collider.x = static_cast<int>(transform->position.x);
			collider.y = static_cast<int>(transform->position.y);
			collider.w = transform->width * static_cast<int>(transform->scale);
			collider.h = transform->height * static_cast<int>(transform->scale);
		}	

		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
	}
};