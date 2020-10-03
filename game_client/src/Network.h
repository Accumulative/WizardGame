#pragma once
#include <SDL2/SDL_net.h>
#include <iostream>
#include <cstring>
#include "ECS/ECS.h"
#include "../../shared/Vector2D.h"

enum class Event {
  Battle, Attack, Decline
};

class Network {
  SDLNet_SocketSet server;
  TCPsocket connection;
  char tmp[1400];
  private:
    int myId;
    int waiter;
    Vector2D oldPos;
    bool waitingForMapChange = false;
  public: 
    Network(const char* ipchar);
    ~Network();
    void send(Entity* e);
    void SendPlayerEvent(Entity* e, int playerid, Event event, int arg1 = 0, int arg2 = 0, int arg3 = 0);
    void SendMessage(const char message[1400]);
    void recv(Manager* manager, Entity* e);
    void ReceiveEvent(const char message[1400], Entity* e);
    void SwitchMap(Entity* e, int mapId, Vector2D newPos);
};
