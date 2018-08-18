#include "Collision.h"
#include "ECS/ColliderComponent.h"
#include "../../shared/Vector2D.h"

bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB, const Vector2D& velocity)
{
	if (
		recA.x + recA.w + velocity.x * 3 >= recB.x &&
		recB.x + recB.w >= recA.x + velocity.x * 3 &&
		recA.y + recA.h + velocity.y * 3 >= recB.y &&
		recB.y + recB.h >= recA.y + velocity.y * 3
		)
	{
		return true;
	}

	return false;
}
bool Collision::MouseAA(const Vector2D mouse, const SDL_Rect& recA)
{
	// std::cout << mouse.x << " " << mouse.y << " " << recA.x << " " << recA.y << " " << recA.h
	// << " " <<recA.w << std::endl;
	if (
		recA.x + recA.w >= mouse.x &&
		recA.x <= mouse.x  &&
		recA.y + recA.h >= mouse.y &&
		recA.y <= mouse.y
		)
	{
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB, const Vector2D& velocity)
{
	if (AABB(colA.collider, colB.collider, velocity))
	{
		//std::cout << colA.tag << " hit: " << colB.tag << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}