#include <string>
#include "Battle.h"
#include <iostream>
#include "Game.h"
#include "ECS/ECS.h"
#include "Menu.h"
#include "Spells.h"
#include "ECS/NetworkComponent.h"

extern Manager manager;

Battle::Battle(std::string tID, Network * net) {
	texture = Game::assets->GetTexture(tID);
	playerOneBar = Game::assets->GetTexture("playerbar");
	playerTwoBar = Game::assets->GetTexture("playerbar");
	battleNet = net;
}
Battle::~Battle() {
	delete playerOneMenu;
}
void Battle::StartBattle(Entity* playerOne, Entity* playerTwo) {
	playerOneEnt = &manager.addEntity();
	playerTwoEnt = &manager.addEntity();
	this->playerOne = playerOne;
	this->playerTwo = playerTwo;
	BattleComponent *playerOneB = &playerOne->getComponent<BattleComponent>();
	BattleComponent *playerTwoB = &playerTwo->getComponent<BattleComponent>();
	std::cout << "Starting battle between " << playerOneB->getName() << " and " << playerTwoB->getName() << std::endl;
	playerOneEnt->addComponent<TransformComponent>(0, 640-32*5, 32, 32, 5, 0);
	playerTwoEnt->addComponent<TransformComponent>(800-32*5, 0, 32, 32, 5, 0);
	playerOneEnt->addComponent<SpriteComponent>(playerOneB->getTexture(), true, false);
	playerOneEnt->getComponent<SpriteComponent>().addAnimation(0, 3, "Idle", 200);
	playerOneEnt->getComponent<SpriteComponent>().Play("Idle");
	playerTwoEnt->addComponent<SpriteComponent>(playerTwoB->getTexture(), true, false);
	playerTwoEnt->getComponent<SpriteComponent>().addAnimation(0, 3, "Idle", 200);
	playerTwoEnt->getComponent<SpriteComponent>().Play("Idle");
	playerOneMenu = new Menu<Spell>("hover", { 
		{ "Fire", Spell::Fire },
		{ "Ice", Spell::Ice },
		{ "Electric", Spell::Electric },
		{ "Water", Spell::Water }
	}, "", 2 );
	SDL_Rect pos = { 200, 250, 400, 240 };
	playerOneMenu->LoadMenu(pos);
}
bool Battle::Finished() {
	return (playerOne->getComponent<BattleComponent>().getHealth() == 0 || playerTwo->getComponent<BattleComponent>().getHealth() == 0);
}
void Battle::PollEvents(SDL_Event event) {
	BattleComponent *playerOneB = &playerOne->getComponent<BattleComponent>();
	if(playerOneB->getTurn()) {
		this->playerOneMenu->PollEvents(event);
		Spell sel = playerOneMenu->GetSelectedOption();
		if(sel != Spell::None) {
			BattleComponent *playerTwoB = &playerTwo->getComponent<BattleComponent>();
			Spell selSpell = static_cast<Spell>(sel);
			int damage = Spells::getDamage(selSpell);
			playerTwoB->dealDamage(damage);
			playerOneB->setTurn(false);
			battleNet->SendPlayerEvent(playerOne, playerTwo->getComponent<NetworkComponent>().getId(), Event::Attack, static_cast<int>(sel));
		}
	}
}

void Battle::draw() {
	BattleComponent *playerOneB = &playerOne->getComponent<BattleComponent>();
	SDL_Rect srcRect, destRect;
	srcRect.x = srcRect.y = 0;
	srcRect.w = srcRect.h = Game::TILE_SIZE;
	destRect.x = 0;
	destRect.y = 0;
	destRect.h = 640;
	destRect.w = 800;
	TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	if(playerOneB->getTurn()) {
		playerOneMenu->DrawMenu();
	}
	int playerOneHealthVar = playerOneB->getHealth();
	int playerTwoHealthVar = playerTwo->getComponent<BattleComponent>().getHealth();
	playerOneEnt->getComponent<SpriteComponent>().draw();
	playerTwoEnt->getComponent<SpriteComponent>().draw();
	TextureManager::Draw(playerTwoBar, { 0, 0, 32, 3 }, { 800-32*8, 32*5-24, 400, 32 }, SDL_FLIP_NONE);
	SDL_SetRenderDrawColor(Game::renderer, 255,0,0,255); 
	SDL_Rect playerOneHealthTotal = { 500, 200, 200, 50 };
	SDL_Rect playerOneHealth = { 500, 200, 200 * playerOneHealthVar/100, 50 };
	SDL_RenderFillRect(Game::renderer, &playerOneHealth);
	SDL_RenderDrawRect(Game::renderer, &playerOneHealthTotal);
	SDL_Rect playerTwoHealthTotal = { 100, 100, 200, 50 };
	SDL_Rect playerTwoHealth = { 100, 100, 200 * playerTwoHealthVar/100, 50 };
	SDL_RenderDrawRect(Game::renderer, &playerTwoHealthTotal);
	SDL_RenderFillRect(Game::renderer, &playerTwoHealth);
}
