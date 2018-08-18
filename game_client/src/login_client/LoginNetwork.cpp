#include "LoginNetwork.h"
#include <NetworkHelper.h>

	LoginNetwork::LoginNetwork(const char* ipchar) {
		SDLNet_Init();
		IPaddress ip;
		myId = -1;
		loggedIn = false;
		if(SDLNet_ResolveHost(&ip, ipchar, 1235) ==-1)
		{
			std::cout << "Network error" <<std::endl;
		}
		connection = SDLNet_TCP_Open(&ip);
		if(connection ==NULL) {
			std::cout << "Error connection (bad ip)" << std::endl;
		}
		server = SDLNet_AllocSocketSet(1);
		SDLNet_TCP_AddSocket(server, connection);
	}
	LoginNetwork::~LoginNetwork() {
		sprintf(tmp, "2 %d \n", myId);
		NetworkHelper::SendMessage(tmp, connection);
		SDLNet_TCP_Close(connection);
		SDLNet_FreeSocketSet(server);
		SDLNet_Quit();
	}
	void LoginNetwork::Login(const char * username, const char * password) {
		sprintf(tmp, "1 %s %s \n", username, password); 
		NetworkHelper::SendMessage(tmp, connection);
	};
	void LoginNetwork::LogState(const char * username, player_data data) {
		sprintf(tmp, "3 %s %d %d %d \n", username, data.map_id, data.xpos, data.ypos); 
		std::cout << "Sending state: " << tmp << std::endl; 
		NetworkHelper::SendMessage(tmp, connection);
	};
	void LoginNetwork::Register(const char * username, const char * password) {
		sprintf(tmp, "2 %s %s \n", username, password); 
		NetworkHelper::SendMessage(tmp, connection);
	};
	void LoginNetwork::recv(LoginState& state) {
		while(SDLNet_CheckSockets(server, 0) > 0 && SDLNet_SocketReady(connection)) {
			int offset = 0;
			do {
				offset += SDLNet_TCP_Recv(connection, tmp+offset, 1400);
				if(offset<=0)
					return;
			}while(tmp[strlen(tmp)-1] != '\n');
			int type;
			sscanf(tmp, "%d", &type);
			int success;
			switch(type) {
				case 1:
					sscanf(tmp, "%d %d", &type, &success);
					if(success == 0) {
						int xpos, ypos;
						loggedIn = true;
						sscanf(tmp, "%d %d %d %d %d \n", &type, &success, &p_data.map_id, &p_data.xpos, &p_data.ypos);
					} else {
						loggedIn = false;
					}
					break;
				case 2:
					sscanf(tmp, "%d %d", &type, &success);
					if(success == 0) {
						state = LoginState::Login;
					}
					break;
			}
		}
	};
	const bool LoginNetwork::IsLoggedIn() {
		return loggedIn;
	}
	void LoginNetwork::ReceiveEvent(char message[1400]) {
		int tmp1, tmp2, tmp3, tmp4;
		sscanf(tmp, "4 %d %d %d", &tmp1, &tmp2, &tmp3);
		std::cout << "Net event " << tmp << std::endl;
	}