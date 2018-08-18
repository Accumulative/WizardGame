#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{
	SDL_Surface* tempSurface = IMG_Load(texture);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
	SDL_FreeSurface(tempSurface);
	
	return tex;
}

void TextureManager::Draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	if(!(Game::camera.w + 100 < dest.x ||
				0 > dest.x + dest.w + 100 ||
				Game::camera.h + 100 < dest.y ||
				0 > dest.y + dest.h + 100))
				{
					SDL_RenderCopyEx(Game::renderer, tex, &src, &dest, 0.0, NULL, flip);
				}
}
