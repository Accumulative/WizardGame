#pragma once

#include "Vector2D.h"

struct player_data {
	int xpos;
	int ypos;
	int map_id;
};

struct NPC_Data {
	int id;
	int mapId;
	std::string name;
	std::string image_name;
	bool canFight;
	Vector2D pos;
};