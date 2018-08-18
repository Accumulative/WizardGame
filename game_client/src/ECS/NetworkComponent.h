#pragma once
#include "ECS.h"

class NetworkComponent : public Component
{
private:
	int id=-1;
	bool loaded = false;
	bool isready;
	bool responding = false;
	int timeout=0;
	bool awaitingPlayer = false;
public:
	NetworkComponent() {};
	NetworkComponent(int id) : id(id) {};
	~NetworkComponent() {}
	void setId(int i) {
		isready = true;
		id=i;
	}
	void setLoaded(bool isLoaded) {
		loaded = isLoaded;
	}
	bool isLoaded() {
		return loaded;
	}
	void setWaitingPlayer(bool waiting) {
		awaitingPlayer	 = true;
	}
	bool getWaitingPlayer() {
		return awaitingPlayer;
	}
	void updateTimeout() {
		if(!responding) {
			timeout++;
			if(timeout > 2000) {
				std::cout << "I am being destroyed " << std::endl;
				entity->destroy();
			}
		} else {
			timeout = 0;
			responding = false;
		}
	}
	int getResponding() {
		return responding;
	}
	int getTimeout() {
		return timeout;
	}
	void setResponding(bool responding) {
		this->responding = responding;
	}
	int getId() {
		return id;
	}
	bool isReady() {
		return isready;
	}
};