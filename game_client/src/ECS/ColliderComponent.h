#pragma once
#include <string>
#include <SDL.h>
#include "ECS.h"
#include "TransformComponent.h"
#include "../TextureManager.h"

enum class ColliderType {
    Event, Terrain, Player, Npc, Projectile
};

class ColliderComponent : public Component
{
public:
	SDL_Rect collider;
	SDL_Rect offset;
	ColliderType tag;

	SDL_Texture* tex;
	SDL_Rect srcR, destR;

	TransformComponent* transform;

	ColliderComponent(ColliderType t)
	{
		tag = t;
	}

	ColliderComponent(ColliderType t, int xpos, int ypos, int xsize, int ysize)
	{
		tag = t;
		collider.x = offset.x = xpos;
		collider.y = offset.y = ypos;
		collider.w = offset.w = xsize;
		collider.h = offset.h = ysize;
	}


	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("assets/ColTex.png");
		srcR = { 0, 0, 32, 32 };
		destR = { collider.x, collider.y, collider.w, collider.h };

	}

	void update() override
	{
		if (tag != ColliderType::Terrain)
		{
			collider.x = static_cast<int>(transform->position.x) + offset.x;
			collider.y = static_cast<int>(transform->position.y) + offset.y;
		}

		destR.x = collider.x - Game::camera.x;
		destR.y = collider.y - Game::camera.y;
	}

	void draw() override
	{
		if (tag != ColliderType::Player)
		{
			TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
		}
	}

private:

};