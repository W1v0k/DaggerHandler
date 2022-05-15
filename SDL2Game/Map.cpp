#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"

extern Manager manager; //folosim extern deoarece vom defini manager in alta parte(adica in Game.cpp)

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts) { scaledSize = ms * ts; }

Map::~Map(){}

//void Map::LoadMap(std::string path, int sizeX, int sizeY)
//{
//	char tile;
//	std::fstream mapFile;
//	mapFile.open(path);
//
//	for (int y = 0; y < sizeY; ++y) //column
//	{
//		for (int x = 0; x < sizeX; ++x)
//		{		
//			//mapFile.get(tile); //functioneaza doar cu numere de la 0 la 9, preia un singur caracter
//			//Game::AddTile(atoi(&tile), x * 32, y * 32); //atoi = ascii to int
//			//mapFile.ignore(); //se ignora virgula
//			std::string tileString;
//			getline(mapFile, tileString, ',');
//			std::cout << tileString<<' ';
//			//int iTS = atoi(tileString.c_str());
//			Game::AddTile(atoi(tileString.c_str()), x * 32, y * 32);
//			//Game::AddTile_v2(iTS / 10, iTS % 10, x * 32, y * 32);
//		}
//		//mapFile.ignore();
//	}
//	mapFile.close();
//}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	/*manager.delGroup(Game::groupMap);
	manager.delGroup(Game::groupColliders);*/
	char c, cc;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX=0, srcY = 0;
	//std::string tileString;

	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; ++x)
		{	
			mapFile.get(c);
			mapFile.get(cc);
			srcY = (atoi(&c) * 10 + atoi(&cc)) * tileSize;
			mapFile.get(c);
			srcX = atoi(&c) * tileSize;
			AddTile(srcX, srcY, x * scaledSize , y * scaledSize);
			mapFile.ignore();
			/*getline(mapFile, tileString, ',');
			std::cout << tileString << ' ';
			int mystoi = atoi(tileString.c_str());*/
			//if (mystoi <= 99)
			//{
				//srcY = (mystoi / 10) * 32;
				//srcX = (mystoi % 10) * 32;
			//}
			//else if(mystoi >= 100 && mystoi <= 999)
			//{
				/*srcY = (mystoi / 10) * 32;
				srcX = (mystoi % 10) * 32;*/
			//}

			/*mapFile.get(c);
			std::cout << c;
			srcY = atoi(&c) * 32;
			mapFile.get(c);
			std::cout << c;
			srcX = atoi(&c) * 32;
			mapFile.get(c);
			std::cout << c;
			if (c != ' ' && c != ',')
			{
				srcY = ((srcY / 32) * 10 + (srcX / 32)) * 32;
				srcX = atoi(&c) * 32;
				mapFile.ignore();
			}
			std::cout << srcY << " " << srcX;*/
			//mapFile.ignore();
		}
		//mapFile.ignore();
	}
	
	mapFile.ignore(); //avem o linie goala

	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; ++x)
		{
			mapFile.get(c);
			if (c == '1') // este collider
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("terrain", x * scaledSize, y * scaledSize, scaledSize);
				tcol.addGroup(Game::groupColliders);
			}
			//2 - primul nivel, 3-al doilea, 4-al treilea, 5-al patrulea, 6-al cincilea
			// este collider, dar identificator de nivel
			if (c == '2' || c == '3' || c == '4' || c == '5' || c == '6')
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("levelChange", c ,x * scaledSize, y * scaledSize, scaledSize);
				//tcol.addComponent<ColliderComponent>().level = atoi(&c);
				tcol.addGroup(Game::groupColliders);
			}

			if (c == '9') //este collider, dar este coin
			{
				auto& tcol(manager.addEntity());
				tcol.addComponent<ColliderComponent>("coin", x * scaledSize, y * scaledSize, 22);
				tcol.addGroup(Game::groupColliders);
			}
			mapFile.ignore(); //daca este 0 trebuie ignorat
		}
	}

	mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos,tileSize, mapScale, texID);
	tile.addGroup(Game::groupMap);
}