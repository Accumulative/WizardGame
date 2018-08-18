#pragma once

#include "ECS.h"
#include "../../../shared/Vector2D.h"
#include "../Game.h"
#include "../TextureManager.h"
#include "../AssetManager.h"
#include "../Map.h"

class TileComponent : public Component
{
public:

	SDL_Texture * texture;
	SDL_Rect srcRect, destRect;
	Vector2D position;
	TileLayer layer;

	TileComponent() = default;

	~TileComponent()
	{
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id, TileLayer layer)
	{
		texture = Game::assets->GetTexture(id);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.w = srcRect.h = tsize;
		position.x = static_cast<float>(xpos);
		position.y = static_cast<float>(ypos);
		destRect.w = destRect.h = tsize * tscale;
		this->layer = layer;
	}

	void update() override
	{
		destRect.x = static_cast<int>(position.x - Game::camera.x);
		destRect.y = static_cast<int>(position.y - Game::camera.y);
	}
	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};