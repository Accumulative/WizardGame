#pragma once
#include <SDL2/SDL_net.h>

class NetworkHelper {
  public:
    void const static SendMessage(const char message[1400], TCPsocket connection) {
      int size    = 0;
      auto len     = static_cast<int>(strlen(message) + 1);
      while(size < len) {
        size+=SDLNet_TCP_Send(connection, message+size, len-size);
      }
    };
};
