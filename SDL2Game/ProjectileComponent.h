#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class ProjectileComponent : public Component 
{
private:
	TransformComponent* transform;
	int range = 0;
	int speed = 0;
	int distance = 0; //counter pentru cat de departe au parcurs proiectilele
	Vector2D velocity;

public:
	ProjectileComponent(int rng, int sp, Vector2D vel) :range(rng), speed(sp), velocity(vel) {} //range, speed
	~ProjectileComponent() {};
	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}
	void update() override //cat de departe a parcurs si daca e afara din boundries
	{
		distance += speed;

		if (distance > range)
		{
			std::cout << "Out of range" << std::endl;
			entity->destroy();
		}
		else if(transform->position.x > Game::camera.x + Game::camera.w || //afara din ecran pe dreapta
				transform->position.x < Game::camera.x || //afara din ecran pe stanga
				transform->position.y > Game::camera.y + Game::camera.h || //afara de pe marginea de jos
				transform->position.y < Game::camera.y) //afara de sus
				//este in range dar trebuie sa verificam daca este inca pe window
		{
			std::cout << "Out of bounds" << std::endl;
			entity->destroy();
		}
	}
};