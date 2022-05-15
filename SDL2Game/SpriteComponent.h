#pragma once

#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map> //hashTable
#include "AssetManager.h"

class SpriteComponent : public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	bool animated = false;
	int frames = 0;
	int speed = 100; //delay between frames in miliseconds

public:

	int animIndex = 0;

	std::map<const char*, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE; //in momentul in care apasam a, mergem in stanga, dorim sa intoarcem playerul

	SpriteComponent() = default;
	SpriteComponent(std::string id)
	{
		setTex(id);
	}

	SpriteComponent(std::string id, bool isAnimated)
	{
		animated = isAnimated;

		Animation idle = Animation(0, 10, 150);
		Animation walk = Animation(1, 10, 150);
		Animation gesture = Animation(2, 10, 100);
		//Animation coinFlip = Animation(3, 8, 100);

		animations.emplace("Idle", idle);
		animations.emplace("Walk", walk);
		animations.emplace("Gesture", gesture);
		//animations.emplace("coinFlip", coinFlip);
	
		play("Idle");

		setTex(id);
	}

	void setTex(std::string id)//pentru a da swap la texture
	{
		texture = Game::assets->GetTexture(id);
	}

	~SpriteComponent()
	{
		//SDL_DestroyTexture(texture);//nu mai distrugem textura deoarece poate fi folosita de mai multe entitati
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		srcRect.x = srcRect.y = 0;
		srcRect.w = transform->width;
		srcRect.h = transform->height;
		destRect.w = destRect.h = 16;
	}

	void update() override
	{
		if (animated)
		{
			srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		srcRect.y = animIndex * transform->height;

		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x; //cast explicit la int deoarece Rectu asteapta un int
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
		destRect.w = transform->width * static_cast<int>(transform->scale); //am scalat transform->scale ori imaginea
		destRect.h = transform->height * static_cast<int>(transform->scale);
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
	}

	void play(const char* animName)
	{
		frames = animations[animName].frames;
		speed = animations[animName].speed;
		animIndex = animations[animName].index;
	}
};