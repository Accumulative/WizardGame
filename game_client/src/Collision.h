#pragma once
#include <SDL.h>
#include "../../shared/Vector2D.h"

class ColliderComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB, const Vector2D& velocity);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB, const Vector2D& velocity);
	static bool MouseAA(const Vector2D mouse, const SDL_Rect& recA);
};
