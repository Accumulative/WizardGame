#pragma once
#include "SDL.h"
#include <string>
#include "ECS/ECS.h"
#include "../../shared/Vector2D.h"

class Textbox {

public:
	SDL_Rect pos;
	void startinput() ;
	Textbox(int xpos, int ypos);
	~Textbox();

	void PollEvents(SDL_Event e);
	void render();
	void stopinput();
	void SetSelected(bool selected);
	std::string GetText();

private:
	bool quit;
	SDL_Event e;
	bool renderText;
	SDL_Color textColor;
	std::string inputText;
	Entity * uiLabel;
	bool selected;
	
};