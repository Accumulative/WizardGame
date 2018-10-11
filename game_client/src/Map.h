#pragma once
#include <string>
#include "../../shared/Vector2D.h"

enum class TileEventType {
	None = -1, Collide, ChangeMap
};

enum class TileLayer {
	Back, Middle, Front
};

class Map 
{
public:
	 
	Map(std::string tID, int ms, int ts);
	~Map();

	void LoadMap(int path, int sizeX, int sizeY, int tileSetSizeX, int tileSetSizeY);
	void AddTile(int src, int xpos, int ypos, TileLayer layer);
	void Remove();

private:
	std::string texID;
	int mapScale;
	int tileSize;
	int scaledSize;
	int tileSetSizeX;
	int tileSetSizeY;

};

struct MapData {
	int mapId;
	Vector2D pos;
	int xSize;
	int ySize;
};

inline MapData GetMapData(int mapid) {
	switch(mapid) {
		case 1:
			return MapData { 1, Vector2D { 0 , 0 }, 68, 42 };
			break;
		case 2:
			return MapData { 2, Vector2D { 0 , 0 }, 22, 17 };
			break;
	}
	return MapData { 1, Vector2D { 0, 0 } , 0 ,0};
}

inline MapData GetMapId(int currMap, int x, int y) {
	MapData data;
	
	switch(currMap) {
		case 1:
			if(x == 160 && y == 192) {
				data = GetMapData(2);
				data.pos = Vector2D { 4 , 15 };
				return data;
			}
			break;
		case 2:
			if(y > 485) {
				data = GetMapData(1);
				data.pos = Vector2D { 5 , 7 };
				return data;
			}
			break;
	}
	return GetMapData(1);
}