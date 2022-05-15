#include "LevelManager.h"

void LevelManager::mapChange(const ColliderComponent& col, const SDL_Rect& recLvlChange, const SDL_Rect& rec, int nrLevel)
{
	if (Collision::AABB(recLvlChange, rec) && col.tag == "levelChange" && col.level == nrLevel)
	{
		if (Game::playerCoins >= nrLevel - 3)
		{
			std::cout << "Poti intra!\n";		
		}
	}
}