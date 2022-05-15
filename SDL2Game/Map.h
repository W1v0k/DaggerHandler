#pragma once
#include <string>

class Map
{
private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;
public:
	Map(std::string tID, int ms, int ts); //texture ID, map scale, tile size
	~Map();

	void LoadMap(std::string path, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xpos, int ypos);
	//static void LoadMap_v2(std::string path, int sizeX, int sizeY);
};