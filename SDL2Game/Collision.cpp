#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		)
	{
		return true;
	}
	return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB)
{
	if (colA.tag != "levelChange" && colB.tag != "levelChange")
	{
		if (AABB(colA.collider, colB.collider))
		{
			return true;
		}
		return false;
	}
	else if (colA.tag != "levelChange" && colB.tag == "levelChange")
	{
		if (AABBLevel(colB.collider, colA.collider))
		{
			std::cout << "Am intrat aici\n";
			return true;
		}
		return false;
	}
	else if (colA.tag == "levelChange" && colB.tag != "levelChange")
	{
		if (AABBLevel(colA.collider, colB.collider))
		{
			std::cout << "Am intrat aici x2";
			return true;
		}
		return false;
	}
}

bool Collision::AABBLevel(const SDL_Rect& recLvlChange, const SDL_Rect& rec)
{
	if (rec.x + rec.w >= recLvlChange.x + recLvlChange.w/2 &&
		recLvlChange.x + recLvlChange.w/2 >= rec.x &&
		rec.y + rec.h >= recLvlChange.y &&
		recLvlChange.y + recLvlChange.h >= rec.y
		)
	{
		std::cout << "Coliziune!\n";
		return true;
	}
	return false;
}
