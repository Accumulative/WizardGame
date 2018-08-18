#pragma once

#include "SDL2/SDL.h"
#include "../Game.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <vector>
#include "../Menu.h"
#include "../Textbox.h"
#include "LoginNetwork.h"
#include "login_struct.h"

enum class LoginOptions {
	None = 0, Login, Options, ExitGame, Register, SubmitRegister, BackToLogin
};

class Login
{
public:
	Login(Game * game);
	~Login();

	void handleEvents();
	void update();
	void render();
	void SavePlayerState(player_data data);

private:
	Menu<LoginOptions> * loginMenu;
	Menu<LoginOptions> * registerMenu;
	Game * game;
	Textbox * usernameTxt;
	Textbox * passwordTxt;
	Vector2D mouse;
	LoginNetwork *net;
	LoginState state;
};