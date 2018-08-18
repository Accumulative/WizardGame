#pragma once
#include <string>
#include "ECS/ECS.h"
#include "ECS/BattleComponent.h"
#include "Network.h"
#include "Menu.h"
#include "Spells.h"

class Battle 
{
public:
	 
	Battle(std::string tID, Network * net);
	~Battle();
	void PollEvents(SDL_Event event);
	void StartBattle(Entity* playerOne, Entity* playerTwo);
	void draw();
	bool Finished();

private:
	SDL_Texture * texture;
	SDL_Texture * playerOneBar;
	SDL_Texture * playerTwoBar;
	Entity * playerOne;
	Entity * playerTwo;
	Entity * playerOneEnt;
	Entity * playerTwoEnt;
	Menu<Spell> * playerOneMenu;
	Network * battleNet;
	BattleState state;
};
