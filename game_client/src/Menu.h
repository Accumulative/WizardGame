#pragma once
#include <string>
#include <map>
#include "ECS/ECS.h"
#include "ECS/SpriteComponent.h"
#include "ECS/TransformComponent.h"
#include "ECS/UILabel.h"
#include <vector>
#include "SDL.h"

extern Manager manager;

template<typename T>
struct data {
	std::string s;
	T n;
};

template<typename T>
class Menu 
{
public:
	 
	Menu(std::string tID, std::vector<std::pair<std::string, T>> options, std::string text, int columns, Vector2D optionOffset = {0,0}) {
		texId = tID;
		this->text = text;
		this->columns = columns;
		this->options = options;
		this->optionOffset = optionOffset;
		selectedOption = 0;
		selected = false;
		menuChangedCooldown = false;
	}

	~Menu() {};

	void RemoveCooldown() {
		if(loaded) {
			menuChangedCooldown = false;
		}
	}
	bool IsLoaded() {
		return loaded;
	}


	void LoadMenu(SDL_Rect pos){
		if(!loaded) {
			this->position = pos;
			menuSprite = &manager.addEntity();
			menuSprite->addComponent<TransformComponent>(pos.x, pos.y, 24, 17, 640/24, 0);
			menuSprite->addComponent<SpriteComponent>(texId, true, false);
			menuSprite->getComponent<SpriteComponent>().addAnimation(0, 1, "Idle", 100);
			menuSprite->getComponent<SpriteComponent>().addAnimation(0, 9, "Animated", 100);
			menuSprite->getComponent<SpriteComponent>().Play("Animated");
			AddItem(text, pos.x + 30, pos.y + 50, false, true );
			int count = 0;

			for(const std::pair<std::string, T> option : options) {
				AddItem(option.first, pos.x + optionOffset.x + 30 + (count % columns) * 150, pos.y + optionOffset.y + 40 + 50 * (count+2 - (count % columns)), true, count == 0 ? true : false);
				count ++;
			}
			loaded = true;
		}
	}

	void SetText(std::string text) {
		this->text = text;
	}

	void LoadMenu()
 {
		this->LoadMenu({ (800-(640/24)*17)/2, 0, 320, 320 });
	}
	
	void AddItem(std::string title, int xpos, int ypos, bool selectable, bool animate){
		if(title != "") {
			auto& label(manager.addEntity());
			label.addComponent<TransformComponent>(xpos, ypos, 40, 270, 1.5, 0);
			label.addComponent<SpriteComponent>("menuButton", true, false);
			label.getComponent<SpriteComponent>().addAnimation(0, 1, "Idle", 100);
			label.getComponent<SpriteComponent>().addAnimation(0, 8, "Animated", 100);
			if(animate) {
				label.getComponent<SpriteComponent>().Play("Animated");
			} else { 
				label.getComponent<SpriteComponent>().Play("Idle");
			}
			SDL_Color white = { 255, 255,255,255};
			SDL_Color red = { 255, 0,0,255};
			label.addComponent<UILabel>(xpos+120, ypos+20, title, "arial", animate ? red : white, 200, selectable);
			labels.push_back(&label);
		}
	}
	void ChangeSelOption(int8_t direction) {
		if(loaded) { 
			if(!menuChangedCooldown)
			{
				switch(direction) {
					case 1: 
						selectedOption = (selectedOption + columns) % (options.size());
						break;
					case 2:
						selectedOption = (selectedOption - columns + (options.size())) % (options.size());
						break;
					case 4:
						selectedOption = (selectedOption + 1) % (options.size());
						break;
					case 3:
						selectedOption = (selectedOption - 1 + (options.size())) % (options.size());
						break;
				}
				int count = 0;
				menuChangedCooldown = true;
				for(auto* label: labels) {
					if(label->template getComponent<UILabel>().IsSelectable()) {
						if(count == selectedOption) {
							label->template getComponent<SpriteComponent>().Play("Animated");
							label->template getComponent<UILabel>().SetLabelColor({255,0,0,255});
						} else {
							label->template getComponent<SpriteComponent>().Play("Idle");
							label->template getComponent<UILabel>().SetLabelColor({255,255,255,255});
						}
					count++;
					}
				}
			}
		}
	}
	void Update() {
		if(loaded) {
			menuSprite->update();
			for(auto* label: labels) {
				label->update();
			}
		}
	}
	void DrawMenu() {
		if(loaded) {
			menuSprite->draw();
			for(auto* label: labels) {
				label->draw();
			}
		}
	};
	
	T GetSelectedOption() {
		if(selected) {
			selected = false;
			return options[selectedOption].second;
		}
		return static_cast<T>(0);
	};
	void PollEvents(SDL_Event event) {
		if(loaded) {
			switch (event.type)
			{
				case SDL_KEYDOWN :
					switch (event.key.keysym.sym)
					{
						case SDLK_DOWN:
								this->ChangeSelOption(1);
							break;
						case SDLK_UP:
								this->ChangeSelOption(2);
							break;
						case SDLK_LEFT:
								this->ChangeSelOption(3);
							break;
						case SDLK_RIGHT:
								this->ChangeSelOption(4);
							break;
						case SDLK_KP_ENTER:
						case SDLK_RETURN:
						case SDLK_RETURN2:
							selected = true;
							break;
					}
					break;
				case SDL_KEYUP :
					case SDLK_DOWN:
					case SDLK_UP:
						this->RemoveCooldown();
						break;
				default:
					break;
			}
		}
	}
	
	void Remove() {
		if(loaded) {
			for(auto* label: labels) {
				label->destroy();
			}
			labels.clear();
			loaded = false;
			menuSprite->destroy();
		}
	}

private:
	std::string texId;
	std::string text;
	std::vector<std::pair<std::string, T>> options;
	Entity* menuSprite;
	std::vector<Entity*> labels;
	bool loaded = false;
	int selectedOption;
	bool selected;
	bool menuChangedCooldown;
	int columns;
	SDL_Rect position;
	Vector2D optionOffset;
};