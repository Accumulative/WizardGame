#pragma once
#include "ECS.h"
#include "ColliderComponent.h"

class EventComponent : public Component
{
public:
	Vector2D position;
	int mapId = 0;
	int eventId = 0;

	EventComponent()
	{
		position.Zero();
	}

	EventComponent(float x, float y, int eventId)
	{
		position.x = x;
		position.y = y;
		this->eventId = eventId;
	}

	EventComponent(float x, float y, int eventId, int mapId)
	{
		this->position.x = x;
		this->position.y = y;
		this->eventId = eventId;
		this->mapId = mapId;
	}

	void init() override
	{
		if (!entity->hasComponent<ColliderComponent>())
		{
			entity->addComponent<ColliderComponent>(ColliderType::Event, static_cast<int>(position.x), static_cast<int>(position.y), 32, 32);
			entity->addGroup(Game::groupColliders);
		}
	}
	void update() override
	{
		
	}

};