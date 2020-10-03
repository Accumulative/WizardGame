#include "../../shared/NetworkHelper.h"
#include "../../shared/Vector2D.h"
#include "../../shared/string_helper.h"
#include "npc.h"
#include <SDL2/SDL_net.h>
#include <iostream>
#include <sstream>
#include <vector>

struct data {
  TCPsocket socket;
  Uint32 timeout;
  int id;
  int mapId;
  Vector2D pos;
  data(TCPsocket sock, Uint32 t, int m, Vector2D pos, int i)
      : socket(sock), timeout(t), mapId(m), pos(pos), id(i) {}
};

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDLNet_Init();
  int curid = 0;
  int playernum = 0;
  sqlite3 *db;

  if (sqlite3_open("../test.db", &db) != SQLITE_OK) {
    fprintf(stderr, "Error opening database.\n");
    return 2;
  }

  NPC *npc_manager = new NPC();
  npc_manager->getAllNpcs(db);

  SDL_Event event;
  IPaddress ip;
  SDLNet_ResolveHost(&ip, nullptr, 1234);
  std::vector<data> socketvector;
  char tmp[1400];
  bool running = true;
  SDLNet_SocketSet sockets = SDLNet_AllocSocketSet(30);
  // SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED,
  // SDL_WINDOWPOS_CENTERED, 600, 400, NULL); SDL_Renderer *renderer =
  // SDL_CreateRenderer(window, -1, 0);
  TCPsocket server = SDLNet_TCP_Open(&ip);

  while (running) {
    // npc_manager->update();

    while (SDL_PollEvent(&event))
      if (event.type == SDL_QUIT ||
          (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        running = false;
    TCPsocket tmpsocket = SDLNet_TCP_Accept(server);
    if (tmpsocket) {
      if (playernum < 30) {
        SDLNet_TCP_AddSocket(sockets, tmpsocket);
        socketvector.push_back(data(tmpsocket, SDL_GetTicks(), 1,
                                    Vector2D(515.0f, 709.0f), curid));
        playernum++;
        sprintf(tmp, "0 %d \n", curid);
        std::cout << "New connection: " << curid << std::endl;
        curid++;
      } else {
        // sprintf(tmp, "3 \n");
        // HANDLE LOBBY
      }
      NetworkHelper::SendMessage(tmp, tmpsocket);
    }
    // check for incoming data
    while (SDLNet_CheckSockets(sockets, 0) > 0) {
      for (int i = 0; i < socketvector.size(); i++) {
        if (SDLNet_SocketReady(socketvector[i].socket)) {

          int newData = 0;
          do {
            newData +=
                SDLNet_TCP_Recv(socketvector[i].socket, tmp + newData, 1400);
            if (newData <= 0)
              break;
          } while (tmp[strlen(tmp) - 1] != '\n');

          if (newData != 0) {
            socketvector[i].timeout = SDL_GetTicks();
            switch (tmp[0]) {
              case '1': {
                int mapId;
                Vector2D vel;
                Vector2D pos;
                int tmp_id;
                sscanf(tmp, "1 %d %d %f %f %f %f \n", &tmp_id, &mapId, &pos.x,
                      &pos.y, &vel.x, &vel.y);
                socketvector[i].pos = pos;
                socketvector[i].mapId = mapId;
                for (int k = 0; k < socketvector.size(); k++) {
                  if (k != i) {
                    if (socketvector[i].mapId == socketvector[k].mapId) {
                      if (fabsf(socketvector[k].pos.x - pos.x) < 1000 &&
                          fabsf(socketvector[k].pos.y - pos.y) < 1000) {
                        NetworkHelper::SendMessage(tmp, socketvector[k].socket);
                      }
                    }
                  }
                }
                break;
              }
              case '2': {
                for (int k = 0; k < socketvector.size(); k++) {
                  if (k == i)
                    continue;
                  NetworkHelper::SendMessage(tmp, socketvector[k].socket);
                }
                SDLNet_TCP_DelSocket(sockets, socketvector[i].socket);
                SDLNet_TCP_Close(socketvector[i].socket);
                socketvector.erase(socketvector.begin() + i);
                playernum--;
                break;
              }
              case '3': {
                int tmp1, xpos, ypos, mapid;
                sscanf(tmp, "3 1 %d %d %d %d \n", &tmp1, &mapid, &xpos, &ypos);
                socketvector[i].mapId = mapid;
                socketvector[i].pos =
                    Vector2D(static_cast<float>(xpos), static_cast<float>(ypos));
                std::cout << "Checking for npcs: " << mapid << " "
                          << socketvector[i].pos << std::endl;
                std::ostringstream out;
                for (auto &n : npc_manager->getNpcsOnMap(socketvector[i].mapId)) {
                  replaceAll(n.name, " ", "+");
                  out << "3 1 " << n.id << " " << static_cast<int>(n.pos.x) << " "
                      << static_cast<int>(n.pos.y) << " " << n.name.c_str() << " "
                      << n.image_name.c_str() << " " << n.canFight << " |";
                }
                out << "3 2 \n";
                NetworkHelper::SendMessage(out.str().c_str(),
                                          socketvector[i].socket);
                std::cout << "Complete map join with: " << out.str() << std::endl;
                break;
              }
              case '4': {
                std::cout << " after receive: " << i << " " << tmp << std::endl;
                int tmpvar1, tmpvar2, tmpvar3;
                sscanf(tmp, "4 %d %d %d \n", &tmpvar1, &tmpvar2, &tmpvar3);
                for (int k = 0; k < socketvector.size(); k++) {
                  if (socketvector[k].id == tmpvar2) {
                    std::cout << "sending battl req to " << tmpvar2 << std::endl;
                    NetworkHelper::SendMessage(tmp, socketvector[k].socket);
                    break;
                  }
                }
                break;
              }
            }
          } else {
            if (SDL_GetTicks() - socketvector[i].timeout > 1000) {
              std::cout << "Disconnecting player: " << socketvector[i].id
                        << std::endl;
              sprintf(tmp, "2 %d \n", socketvector[i].id);
              for (int k = 0; k < socketvector.size(); k++) {
                if (k != i) {
                  NetworkHelper::SendMessage(tmp, socketvector[k].socket);
                }
              }
              SDLNet_TCP_Close(socketvector[i].socket);
              SDLNet_TCP_DelSocket(sockets, socketvector[i].socket);
              socketvector.erase(socketvector.begin() + i);
              playernum--;
            }
          }
        }
      }
    }
    SDL_Delay(1);
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);
  }
  for (int i = 0; i < socketvector.size(); i++) {
    SDLNet_TCP_Close(socketvector[i].socket);
  }
  delete npc_manager;
  // SDL_DestroyWindow(window);
  // SDL_DestroyRenderer(renderer);
  SDLNet_FreeSocketSet(sockets);
  SDLNet_TCP_Close(server);
  SDLNet_Quit();
  SDL_Quit();
}
