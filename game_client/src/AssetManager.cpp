#include "AssetManager.h"
#include "ECS/TransformComponent.h"
#include "ECS/SpriteComponent.h"
#include "ECS/ProjectileComponent.h"
#include "ECS/ColliderComponent.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{
	std::map<std::string, SDL_Texture *>::iterator tex;
	for ( tex = textures.begin(); tex != textures.end(); tex++ )
	{
		SDL_DestroyTexture(tex->second);
	}
}

void AssetManager::CreateProjectile(Vector2D pos, Vector2D vel, int range, int speed, std::string id)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(pos.x, pos.y, Game::TILE_SIZE, Game::TILE_SIZE, Game::SCALE);
	projectile.addComponent<SpriteComponent>(id, false, true);
	projectile.addComponent<ProjectileComponent>(range, speed, vel);
	projectile.addComponent<ColliderComponent>(ColliderType::Projectile);
	projectile.addGroup(Game::groupProjectiles);
}

void AssetManager::AddTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

SDL_Texture* AssetManager::GetTexture(std::string id)
{
	return textures[id];
}

void AssetManager::AddFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}
TTF_Font* AssetManager::GetFont(std::string id)
{
	return fonts[id];
}