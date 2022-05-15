#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component //clasa derivata TransformComponent a clasei de baza Component(inheritance de grad 1)
{
public:

	Vector2D position;
	Vector2D velocity;
	
	int height = 32;
	int width = 32;
	int scale = 1;

	int speed = 3;

	//bool silverKey, brassKey, goldKey;

	TransformComponent()
	{
		position.zero();
		//silverKey = brassKey = goldKey = false;
	}

	TransformComponent(int sc)
	{
		position.x = 512 / 2 + 32;
		position.y = 512 / 2 + 32;
		scale = sc;
		//silverKey = brassKey = goldKey = false;
	}

	TransformComponent(float x, float y)
	{
		position.zero();
		//silverKey = brassKey = goldKey = false;
		//position.x = x;
		//position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
		//silverKey = brassKey = goldKey = false;
	}

	void init() override //init este virtual in clasa de baza, override specifica ca o functie virtuala suprascrie alta functie virtuala
	{
		velocity.zero();
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};