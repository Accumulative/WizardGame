#include "Login.h"
#include "../Game.h"
#include "../Collision.h"
#include "../Textbox.h"
#include "../../../shared/data_struct.h"

Login::Login(Game * game) {
	loginMenu = new Menu<LoginOptions>( "menu", { 
		{ "Login", LoginOptions::Login },
		{ "Register", LoginOptions::Register },
		{ "Options", LoginOptions::Options },
		{ "Exit game", LoginOptions::ExitGame }
	}, "Main menu", 1, {0, 100});
	registerMenu = new Menu<LoginOptions>( "menu", { 
		{ "Submit", LoginOptions::SubmitRegister },
		{ "Back", LoginOptions::BackToLogin }
	}, "Main menu", 1, {0, 100});
	this->game = game;
	loginMenu->LoadMenu({ (800-(640/24)*17)/2, 10, 800, 640 });
	registerMenu->LoadMenu({ (800-(640/24)*17)/2, 10, 800, 640 });
	usernameTxt = new Textbox(325, 150);
	passwordTxt = new Textbox(325, 200);
	passwordTxt->startinput();
	usernameTxt->startinput();
	net = new LoginNetwork("127.0.0.1");
	state = LoginState::Login;
}
Login::~Login() {
	delete loginMenu;
	delete usernameTxt;
	delete net;
}

void Login::handleEvents() {
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0) {
		if(state == LoginState::Login) {
			loginMenu->PollEvents(event);
		} else if(state == LoginState::Register)  {
			registerMenu->PollEvents(event);
		}
		usernameTxt->PollEvents(event);
		passwordTxt->PollEvents(event);
		switch (event.type)
		{
			case SDL_QUIT :
				game->state = GameState::NotRunning;
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse.x = event.motion.x;
				mouse.y = event.motion.y; 
				if(Collision::MouseAA(mouse, usernameTxt->pos)) {
					usernameTxt->SetSelected(true);
					passwordTxt->SetSelected(false);
				} else if(Collision::MouseAA(mouse, passwordTxt->pos)) {
					usernameTxt->SetSelected(false);
					passwordTxt->SetSelected(true);
				} else {
					usernameTxt->SetSelected(false);
					passwordTxt->SetSelected(false);
				}
				break;
			default:
				break;
		}
	}
}
void Login::SavePlayerState(player_data data) {
	this->net->LogState(usernameTxt->GetText().c_str(), data);
}
void Login::update() {
	net->recv(state);
	if(state == LoginState::Login) {
		loginMenu->Update();
		if(net->IsLoggedIn()) {
			game->setPlayerState(net->p_data);
			game->state = GameState::NotConnected;
		} else {
			LoginOptions selectedOption = loginMenu->GetSelectedOption();
			switch(selectedOption) {
				case LoginOptions::Login:
					net->Login(usernameTxt->GetText().c_str(), passwordTxt->GetText().c_str());
					break;
				case LoginOptions::Options:
					state = LoginState::Options;
					break;
				case LoginOptions::Register:
					state = LoginState::Register;
					break;
				case LoginOptions::ExitGame:
					game->state = GameState::NotRunning;
					break;
				default:
					break;
			}
		}
	} else if(state == LoginState::Options) {

	} else if(state == LoginState::Register) {
		registerMenu->Update();
		LoginOptions selectedOption = registerMenu->GetSelectedOption();
		switch(selectedOption) {
			case LoginOptions::SubmitRegister:
				net->Register(usernameTxt->GetText().c_str(), passwordTxt->GetText().c_str());
				break;
			case LoginOptions::BackToLogin:
				state = LoginState::Login;
				break;
			default:
				break;
		}
	}
}
void Login::render() {
	if(state == LoginState::Login) {
		loginMenu->DrawMenu();
		passwordTxt->render();
		usernameTxt->render();
	} else if (state == LoginState::Register) {
		registerMenu->DrawMenu();
		passwordTxt->render();
		usernameTxt->render();
	}
}