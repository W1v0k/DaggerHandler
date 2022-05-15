#pragma once

#include "Collision.h"
#include "ColliderComponent.h"
#include "Components.h"
#include "ECS.h"
#include "Game.h"

class LevelManager
{
private:
public:
	TransformComponent* transform;
	void mapChange(const ColliderComponent& col, const SDL_Rect& recLvlChange, const SDL_Rect& rec, int nrLevel);
};