#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KBController : public Component
{
public:
	TransformComponent* transform;
	SpriteComponent* sprite;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN) //a fost apasata o tasta
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1;
				sprite->play("Walk");
				break;
			case SDLK_UP:
				transform->velocity.y = -1;
				sprite->play("Walk");
				break;
			case SDLK_s:
				transform->velocity.y = +1;
				sprite->play("Walk");
				break;
			case SDLK_DOWN:
				transform->velocity.y = +1;
				sprite->play("Walk");
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				sprite->play("Walk");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL; //orientam playerul corect
				break;
			case SDLK_LEFT:
				transform->velocity.x = -1;
				sprite->play("Walk");
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_d:
				transform->velocity.x = +1;
				sprite->play("Walk");
				break;
			case SDLK_RIGHT:
				transform->velocity.x = +1;
				sprite->play("Walk");
				break;
			case SDLK_f:
				sprite->play("Gesture");
				break;
			default:
				break;
			}
		}

		if (Game::event.type == SDL_KEYUP) //nu se mai apasa pe tasta
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				sprite->play("Idle");
				break;
			case SDLK_UP:
				transform->velocity.y = 0;
				sprite->play("Idle");
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				sprite->play("Idle");
				break;
			case SDLK_DOWN:
				transform->velocity.y = 0;
				sprite->play("Idle");
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				sprite->play("Idle");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_LEFT:
				transform->velocity.x = 0;
				sprite->play("Idle");
				sprite->spriteFlip = SDL_FLIP_NONE;
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				sprite->play("Idle");
				break;
			case SDLK_RIGHT:
				transform->velocity.x = 0;
				sprite->play("Idle");
				break;
			case SDLK_ESCAPE:
				Game::isRunning = false;
			case SDLK_f:
				sprite->play("Idle");
				break;
			default:
				break;
			}
		}
	}
};