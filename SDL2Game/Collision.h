#pragma once
#include <SDL.h>

class ColliderComponent; //pentru a stii ce clasa se va folosi

class Collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB); 
	static bool AABBLevel(const SDL_Rect& recLvlChange, const SDL_Rect& rec);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);
};
