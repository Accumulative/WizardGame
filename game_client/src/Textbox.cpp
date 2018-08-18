#include "Textbox.h"
#include "ECS/ECS.h"
#include "ECS/TransformComponent.h"
#include "ECS/SpriteComponent.h"
#include "ECS/UILabel.h"

extern Manager manager;

//Enable text input
void Textbox::startinput() { 
	SDL_StartTextInput();
}
Textbox::Textbox(int xpos, int ypos) {
	pos.x = xpos;
	pos.y = ypos;
	pos.h = 50;
	pos.w = 200;
	uiLabel = &manager.addEntity();
	inputText = "AAA";
	textColor = { 0, 0, 0, 255 };
	uiLabel->addComponent<UILabel>(xpos+35, ypos, inputText, "arial", textColor, 300, true);
	uiLabel->addComponent<TransformComponent>(xpos, ypos-20, 7, 32, 6, 0);
	uiLabel->addComponent<SpriteComponent>("textbox", true, false);
	uiLabel->getComponent<SpriteComponent>().addAnimation(0, 1, "Idle", 1000);
	uiLabel->getComponent<SpriteComponent>().addAnimation(0, 2, "Active", 1000);
	uiLabel->getComponent<SpriteComponent>().Play("Idle");
	quit = false;
	selected = false;
}

Textbox::~Textbox() {

}
std::string Textbox::GetText() {
	 return inputText;
}

void Textbox::PollEvents(SDL_Event e) {
//The rerender text flag
	renderText = false;
	if(selected) {
		//Handle events on queue
		if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
		{
				//lop off character
				inputText.pop_back();
				renderText = true;
		} else if( e.type == SDL_TEXTINPUT ) {
				//Not copy or pasting
				//Append character
				inputText += e.text.text;
				renderText = true;
		};
	}
}
void Textbox::SetSelected(bool selected) {
	this->selected = selected;
	if(inputText == "" && selected) {
		uiLabel->getComponent<SpriteComponent>().Play("Active");
	} else if(!selected) {
		uiLabel->getComponent<SpriteComponent>().Play("Idle");
	}
}
void Textbox::render(){
	if( renderText )
	{
				uiLabel->getComponent<UILabel>().SetLabelText(inputText);
				if(inputText == "") {
					uiLabel->getComponent<SpriteComponent>().Play("Active");
				} else {
					uiLabel->getComponent<SpriteComponent>().Play("Idle");
				}
	}
	uiLabel->update();
	uiLabel->getComponent<SpriteComponent>().draw();
	uiLabel->getComponent<UILabel>().draw();
}

void Textbox::stopinput(){
	SDL_StopTextInput();
}