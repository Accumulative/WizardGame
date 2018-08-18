#pragma once

#include "ECS.h"
#include "../AssetManager.h"
#include "../Game.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabel : public Component
{
public:
	UILabel(int xpos, int ypos, std::string text, std::string font, SDL_Color& colour, int width, bool selectable) :
		labelText(text), labelFont(font), textColour(colour), width(width), selectable(selectable)
	{
		position.x = xpos;
		position.y = ypos;

		SetLabelText(labelText, labelFont, textColour);

	}
	~UILabel()
	{}

	void SetLabelText(std::string text, std::string font = "arial", SDL_Color color = { 0, 0,0,255})
	{
		SDL_Surface* surf = TTF_RenderText_Blended_Wrapped(Game::assets->GetFont(font), text.c_str(), color, width);
		labelTexture = SDL_CreateTextureFromSurface(Game::renderer, surf);
		SDL_FreeSurface(surf);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}
	void SetLabelColor(SDL_Color color) {
		textColour = color;
		SetLabelText(labelText, labelFont, color);
	}
	void draw() override
	{
		SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
	}
	bool IsSelectable() {
		return selectable;
	}

private:
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	int width;
	SDL_Color textColour;
	SDL_Texture* labelTexture;
	bool selectable;
};
