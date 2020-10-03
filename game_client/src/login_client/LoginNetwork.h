#pragma once
#include <SDL2/SDL_net.h>
#include <iostream>
#include <cstring>
#include "../../../shared/data_struct.h"
#include "login_struct.h"

class LoginNetwork {
	SDLNet_SocketSet server;
	TCPsocket connection;
	char tmp[1400];
	private:
		int myId;
		bool loggedIn;
	public: 
		player_data p_data;
		LoginNetwork(const char* ipchar);
		~LoginNetwork();
		void Login(const char * username, const char * password);
		void Register(const char * username, const char * password);
		void LogState(const char * username, player_data data);
		void recv(LoginState& state);
		void ReceiveEvent(char message[1400]);
		const bool IsLoggedIn();
};