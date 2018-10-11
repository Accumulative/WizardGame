#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>
#include "Network.h"
#include <data_struct.h>


enum class EventTypes {
	None = 0, CloseMenu, InGameOptions, Quit, Fight, Trade, ViewStats, Accept, Decline
};

class AssetManager;
class ColliderComponent;

enum class GameState {
	NotConnected, Connected, Offline, NotRunning, LoggedOut
};
EventTypes inline operator+(EventTypes e1, EventTypes e2) { 
	return static_cast<EventTypes>(static_cast<int>(e1) + static_cast<int>(e2)); 
}

class Game
{
public:
	Game(SDL_Renderer *renderer);
	~Game();

	void init();

	void handleEvents();
	void update();
	void render();
	void setPlayerState(player_data p_data);
    void CheckCollisions();
    player_data getPlayerState();

	GameState state;
	static SDL_Renderer *renderer;
	static SDL_Event event;
	static SDL_Rect camera;
	static AssetManager* assets;
	static const int TILE_SIZE;
	static const int SCALE;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupColliders,
		groupProjectiles,
		groupEvents,
		groupUI,
		groupNPCs
	};

private:

	int cnt = 0;
	bool isOnline = false;
	Network* net;
};