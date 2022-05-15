#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream> //string stream, asociaza un obiect de tip string cu un stream. Se poate citi din string ca un cin
#include "LevelManager.h"

using namespace std;

template<typename... Args>
std::string string_format(const char* fmt, Args... args)
{
	size_t size = snprintf(nullptr, 0, fmt, args...);
	std::string buf;
	buf.reserve(size + 1);
	buf.resize(size);
	snprintf(&buf[0], size + 1, fmt, args...);
	return buf;
}

Map* MyMap;
LevelManager* MyLvlManager;

SDL_Renderer* Game::renderer = nullptr; //initializare

Manager manager;
SDL_Event Game::event;

SDL_Rect Game::camera = { 0, 0, 512, 512 };

AssetManager* Game::assets = new AssetManager(&manager);

int Game::playerCoins = 0;
int Game::levelNumber = 1; //init
//std::vector<ColliderComponent*> Game::colliders;
bool Game::isRunning = false;

Entity* player = 0;
//auto& coin(manager.addEntity());
Entity* label = 0;
//auto& wall(manager.addEntity());
//const char* mapfile = "assets/tileset_ss.png";
//auto& enemies(manager.getGroup(groupEnemies));

Game::Game(){}

Game::~Game(){}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen)
{
	player = &manager.addEntity();
	label = &manager.addEntity();

	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)//s-au initializat cu succes subsistemele SDL
	{
		cout << "Subsystems initialised!..." << endl;

		//create window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		if (window)
		{
			cout << "Window created!..." << endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			cout << "Renderer created!..." << endl;
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	if (TTF_Init() == -1)//eroare
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}
	
	assets->AddTexture("terrain", "assets/tileset_ss.png");
	assets->AddTexture("player", "assets/player/animationsList.png");
	assets->AddTexture("projectile", "assets/proj.png");
	assets->AddTexture("coin", "assets/coin.png");

	assets->AddFont("MATURA", "assets/MATURASC.ttf", 16);
	assets->AddFont("BALOO", "assets/Baloo-Regular.ttf", 16);

	MyMap = new Map("terrain", 2, 16);

	//ecs implementation

	MyMap->LoadMap("assets/llvl3.txt", 32, 32); //pentru incarcarea llvl4.txt si llvl5.txt trebuie modificata pozitia playerului din transform component (se spawneaza direct in collider)
	//Map::LoadMap("assets/level1.txt",32,32);
	player->addComponent<TransformComponent>(2);
	player->addComponent<SpriteComponent>("player", true);
	player->addComponent<KBController>();
	player->addComponent<ColliderComponent>("player");
	player->addGroup(groupPlayers);

	/*coin.addComponent<TransformComponent>(1);
	coin.addComponent<SpriteComponent>("coin", true);
	coin.addComponent<ColliderComponent>("coin");
	coin.addGroup(groupCoins);*/

	SDL_Color white = { 255,255,255,255 };
	label->addComponent<UILabel>(8, 8, "Test String", "BALOO", white);

	assets->CreateProjectile(Vector2D(250, 250), Vector2D(2,0),200, 2, "projectile");
	assets->CreateProjectile(Vector2D(350, 250), Vector2D(2, 1), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(150, 450), Vector2D(2, -1), 200, 2, "projectile");

}



auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));

void Game::handleEvents()
{
	SDL_PollEvent(&event);
	switch (event.type)
	{
	case SDL_QUIT:
		{
		isRunning = false;
		break; 
		}
	default:
	{
		break;
	}
	}
}

void Game::update()
{
	SDL_Rect playerCol = player->getComponent<ColliderComponent>().collider; //la fiecare frame vom dori sa luam Rectangle-ul playerului
	Vector2D playerPos = player->getComponent<TransformComponent>().position; //retinem player pos pentru ca in momentul in care playerul face coliziune sa il fortam la pozitia anterioara, unde nu facea coliziune inca cu collideru

	std::stringstream ss; // poate cauza leaks de memorie
	ss << "Player position: " << playerPos<<" Coins: " << playerCoins; //creare stream
	label->getComponent<UILabel>().SetLabelText(ss.str(), "BALOO");
	

	manager.refresh();
	manager.update(); //face update la entitati => update la toate componentele
	
	for (auto& c : colliders) //verificam daca playerul se loveste de collidere
	{

		SDL_Rect cCol = c->getComponent<ColliderComponent>().collider;
		if (Collision::AABB(cCol, playerCol) && c->getComponent<ColliderComponent>().tag == "terrain" || Collision::AABB(cCol, playerCol) && c->getComponent<ColliderComponent>().tag == "levelChange") //avem coliziune
		{
			player->getComponent<TransformComponent>().position = playerPos;
		}

		//MyLvlManager->mapChange(c->getComponent<ColliderComponent>(), cCol, playerCol, 4);
		if (Collision::AABB(cCol, playerCol) && c->getComponent<ColliderComponent>().tag == "levelChange" && c->getComponent<ColliderComponent>().level == 4)
		{
			std::cout << playerCoins << std::endl;
			if (playerCoins >= 1)
			{
				//std::cout << "Poti intra!";
				//SDL_Delay(5000);
				SDL_RenderClear(renderer);
				manager.wipe();
				/*/assets->AddTexture("terrain", "assets/tileset_ss.png");
				assets->AddTexture("player", "assets/player/animationsList.png");
				assets->AddTexture("projectile", "assets/proj.png");
				assets->AddTexture("coin", "assets/coin.png");

				assets->AddFont("MATURA", "assets/MATURASC.ttf", 16);
				assets->AddFont("BALOO", "assets/Baloo-Regular.ttf", 16);
				*/
				player = &manager.addEntity();
				MyMap->LoadMap("assets/llvl3.txt", 32, 32);
				player->addComponent<TransformComponent>(2);
				player->addComponent<SpriteComponent>("player", true);
				player->addComponent<KBController>();
				player->addComponent<ColliderComponent>("player");
				player->addGroup(groupPlayers);
				SDL_Color white = { 255,255,255,255 };
				label->addComponent<UILabel>(8, 8, "Test String", "BALOO", white);

				assets->CreateProjectile(Vector2D(250, 250), Vector2D(2, 0), 200, 2, "projectile");
				assets->CreateProjectile(Vector2D(350, 250), Vector2D(2, 1), 200, 2, "projectile");
				assets->CreateProjectile(Vector2D(150, 450), Vector2D(2, -1), 200, 2, "projectile");
				return;
				//Game::init("DaggerHandler", 0, 0, 512, 512, 0, 4);
			}
		}
			/*player.getComponent<TransformComponent>().position.x = playerPos.x;
			player.getComponent<TransformComponent>().position.y = playerPos.y - 8;*/
		
			//render();
			//SDL_RenderPresent(renderer);
			//MyMap = new Map("terrain", 2, 16);
			//MyMap->LoadMap("assets/llvl2.txt", 32, 32);
			//SDL_Delay(5000);
		//}
		//else if (Collision::AABB(cCol, playerCol) && c->getComponent<ColliderComponent>().tag == "levelChange" && c->getComponent<ColliderComponent>().level == 5)
		//{
		//	//player.getComponent<TransformComponent>().position.x = playerPos.x - 8;
		//	/*player.getComponent<TransformComponent>().position.y = playerPos.y;*/
		//	if (playerCoins >= 2)
		//	{
		//		std::cout << "Poti intra!";
		//		for (auto& col : colliders)
		//		{
		//			col->delGroup(groupColliders);
		//			col->destroy();
		//		}
		//		for (auto& t : tiles)
		//		{
		//			t->delGroup(groupMap);
		//			t->destroy();
		//		}

		//		Game::init("DaggerHandler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, false, 3);
		//	}
		//}
		//else if (Collision::AABB(cCol, playerCol) && c->getComponent<ColliderComponent>().tag == "levelChange" && c->getComponent<ColliderComponent>().level == 6)
		//{
		//	player.getComponent<TransformComponent>().position.x = playerPos.x;
		//	player.getComponent<TransformComponent>().position.y = playerPos.y + 8;
		//	if (playerCoins >= 1)
		//	{
		//		std::cout << "Poti intra!";
		//		for (auto& col : colliders)
		//		{
		//			col->delGroup(groupColliders);
		//			col->destroy();
		//		}
		//		for (auto& t : tiles)
		//		{
		//			t->delGroup(groupMap);
		//			t->destroy();
		//		}

		//		Game::init("DaggerHandler", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, false, 3);
		//	}
		//}
			if (Collision::AABB(cCol, playerCol) && c->getComponent<ColliderComponent>().tag == "coin")
			{
				playerCoins++;
				c->destroy();
			}
	}
			for (auto& p : projectiles)
			{
				if (Collision::AABB(player->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
				{
					std::cout << "Hit player!" << std::endl;
					p->destroy();
					/*SDL_DestroyRenderer(renderer);
					renderer = SDL_CreateRenderer(window, -1, 0);
					if (renderer)
					{
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
					}
					for (auto& t : tiles)
					{
						t->delGroup(groupMap);
					}*/
					//Game::init("DaggerHandler", 0, 0, 512, 512, 0, 4);
					//MyMap = new Map("terrain", 2, 16);
					//MyMap->LoadMap("assets/llvl4.txt", 32, 32);
					//Game::render();
				}
			}

			camera.x = player->getComponent<TransformComponent>().position.x - 512 / 2;
			camera.y = player->getComponent<TransformComponent>().position.y - 512 / 2;

			//camera boundries
			if (camera.x < 0) { camera.x = 0; }
			if (camera.y < 0) { camera.y = 0; }
			if (camera.x > camera.w) { camera.x = camera.w; }
			if (camera.y > camera.h) { camera.y = camera.h; }

			//Vector2D pVel = player.getComponent<TransformComponent>().velocity;
			//int pSpeed = player.getComponent<TransformComponent>().speed;

			//for (auto t : tiles)
			//{
			//	t->getComponent<TileComponent>().destRect.x += -(pVel.x * pSpeed); //daca mergem spre dreapta, mapa se va duce spre stanga, de aceea -
			//	t->getComponent<TileComponent>().destRect.y += -(pVel.y * pSpeed); 	
			//}

			/*for (auto cc : colliders)
			{
				Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
			}*/
}

void Game::render()
{
	SDL_RenderClear(renderer);
	//bgs, obj, last player
	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto& c : colliders)
	{
		if (c->getComponent<ColliderComponent>().tag == "coin")
		{
			c->draw();
		}
	}

	for (auto& p : players)
	{
		p->draw();
	}
	/*for (auto& e : enemies)
	{
		e->draw();
	}*/

	for (auto& p : projectiles)
	{
		p->draw();
	}
	
	label->draw();

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Game cleaned!..." << endl;
}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool fullscreen, int level)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)//s-au initializat cu succes subsistemele SDL
	{
		cout << "Subsystems initialised!..." << endl;

		//create window
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		if (window)
		{
			cout << "Window created!..." << endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			cout << "Renderer created!..." << endl;
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}

	if (TTF_Init() == -1)//eroare
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}

	assets->AddTexture("terrain", "assets/tileset_ss.png");
	assets->AddTexture("player", "assets/player/animationsList.png");
	assets->AddTexture("projectile", "assets/proj.png");
	assets->AddTexture("coin", "assets/coin.png");

	assets->AddFont("MATURA", "assets/MATURASC.ttf", 16);
	assets->AddFont("BALOO", "assets/Baloo-Regular.ttf", 16);

	MyMap = new Map("terrain", 2, 16);

	//ecs implementation

	std::string buffer = string_format("assets/llvl%d.txt", level);
	MyMap->LoadMap(buffer, 32, 32);
	//MyMap->LoadMap("assets/llvl2.txt", 32, 32);
	//Map::LoadMap("assets/level1.txt",32,32);
	player->addComponent<TransformComponent>(2);
	player->addComponent<SpriteComponent>("player", true);
	player->addComponent<KBController>();
	player->addComponent<ColliderComponent>("player");
	player->addGroup(groupPlayers);

	/*coin.addComponent<TransformComponent>(1);
	coin.addComponent<SpriteComponent>("coin", true);
	coin.addComponent<ColliderComponent>("coin");
	coin.addGroup(groupCoins);*/

	SDL_Color white = { 255,255,255,255 };
	label->addComponent<UILabel>(8, 8, "Test String", "BALOO", white);

	assets->CreateProjectile(Vector2D(250, 250), Vector2D(2, 0), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(350, 250), Vector2D(2, 1), 200, 2, "projectile");
	assets->CreateProjectile(Vector2D(150, 450), Vector2D(2, -1), 200, 2, "projectile");

}

//void Game::updateLevel()
//{
//	switch (levelNumber)
//	{
//	case 1:
//		break;
//	case 2:
//		break;
//	case 3:
//		break;
//	case 4:
//		break;
//	}
//}

//void Game::AddTile(int id, int x, int y)
//{
//	auto& tile(manager.addEntity());
//	tile.addComponent<TileComponent>(x,y,32,32,id);
//	tile.addGroup(groupMap);
//}
