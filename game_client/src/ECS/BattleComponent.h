#pragma once
#include "ECS.h"

enum class BattleState {
	NotStarted, Requested, RequestedOf, Accepted, Declined, InProgress, Finished
};

class BattleComponent : public Component
{
private:
	std::string playername;
	int opponentId = -1;
	std::string tex;
	bool battleStarted = false;
	bool yourTurn = false;
	int health;
	int strength; 
	BattleState state = BattleState::NotStarted;
public:
	BattleComponent(std::string name, std::string tex) : playername(name), tex(tex) {
		health = 100;
	};
	~BattleComponent() {}
	std::string getName() {
		return playername;
	}
	std::string getTexture() {
		return tex;
	}
	void RequestBattle(int opponentId) {
		this->opponentId = opponentId;
	}
	int GetBattleRequest() {
		return opponentId;
	}
	void SetState(BattleState state) {
		this->state = state;
	}
	BattleState GetCurrentState () {
		return state;
	}
	void dealDamage(int damage) {
		if(health > damage) {
			health -= damage;
		} else {
			health = 0;
		}
		std::cout << "Received " << damage << " d. now have " << health << std::endl;
	}
	void resetHealth() {
		health = 100;
	}
	int getHealth() {
		return health;
	}
	bool getTurn() { 
		return yourTurn;
	}
	void setTurn(bool turn) {
		yourTurn = turn;
	}
};