#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS/EventComponent.h"
#include "ECS/TileComponent.h"

extern Manager manager;

Map::Map(std::string tID, int ms, int ts) : texID(tID), mapScale(ms), tileSize(ts)
{
	scaledSize = ms * ts;
}

Map::~Map()
{
}

void Map::LoadMap(int path, int sizeX, int sizeY, int tileSetSizeX, int tileSetSizeY)
{
	char c;
	std::fstream mapFile;
	this->tileSetSizeX = tileSetSizeX;
	this->tileSetSizeY = tileSetSizeY;

	std::cout << "Load map: " << "assets/maps/" + std::to_string(path) + "/Background.csv with size " << sizeX << " and " << sizeY << std::endl;

	for(int z = 0; z < 3; z++) {
		mapFile.open("/home/kieran/git/online-wizard-game/game_client/assets/maps/" + std::to_string(path) + "/" + std::to_string(z) + ".csv");
		for (int y = 0; y < sizeY; y++)
		{
			for (int x = 0; x < sizeX; x++)
			{
				std::string src;
				mapFile.get(c);
				while(c != ',' && c != '\n') {
					src += c;
					mapFile.get(c);
				}
				AddTile( atoi(src.c_str()), x * scaledSize, y * scaledSize, static_cast<TileLayer>(z));
			}
		}
		mapFile.close();
	}
	mapFile.open("/home/kieran/git/online-wizard-game/game_client/assets/maps/" + std::to_string(path) + "/Collision.csv");

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			std::string src;
			mapFile.get(c);
			while(c != ',' && c != '\n') {
				src += c;
				mapFile.get(c);
			}
			TileEventType et = static_cast<TileEventType>(atoi(src.c_str()));
			if(et != TileEventType::None) {
				auto& tcol(manager.addEntity());
				switch(et) {
					case TileEventType::Collide:
						tcol.addComponent<ColliderComponent>(ColliderType::Terrain, x * scaledSize, y * scaledSize, scaledSize, scaledSize);
						tcol.addGroup(Game::groupColliders);
						break;
					case TileEventType::ChangeMap:
						tcol.addComponent<EventComponent>(x * scaledSize, y * scaledSize, 1, path);
						tcol.addGroup(Game::groupEvents);
						break;
					// case other///
					// 	mapFile.ignore();
					// 	mapFile.get(c);
					// 	tcol.addComponent<EventComponent>(x * scaledSize, y * scaledSize, eventId);
					// 	tcol.addGroup(Game::groupEvents);
				}
			}
		}
	}
}

void Map::AddTile(int src, int xpos, int ypos, TileLayer layer)
{
	if(src != -1) {
		auto& tile(manager.addEntity());
		int srcX = src % this->tileSetSizeX;
		int srcY = floor(src / this->tileSetSizeX);
		// std::cout << src << " " << srcX << " " << srcY << " Pos: " << xpos << " y: " << ypos << std::endl;
		tile.addComponent<TileComponent>(srcX * tileSize, srcY * tileSize, xpos, ypos, tileSize, mapScale, texID, layer);
		tile.addGroup(Game::groupMap);
	}
}
void Map::Remove() {
	auto& tiles(manager.getGroup(Game::groupMap));
	auto& colliders(manager.getGroup(Game::groupColliders));
	auto& events(manager.getGroup(Game::groupEvents));
	for (auto& t : tiles)
	{
		// t->delGroup(Game::groupMap);
		t->destroy();
	}
	for (auto& e : events)
	{
		// e->delGroup(Game::groupEvents);
		e->destroy();
	}
	for (auto& c : colliders)
	{
		// c->delGroup(Game::groupColliders);
		c->destroy();
	}
	// tiles.clear();
	// colliders.clear();
	// events.clear();
}