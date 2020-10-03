#include "../../shared/NetworkHelper.h"
#include "../../shared/data_struct.h"
#include "sqlite3.h"
#include <SDL2/SDL_net.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>

struct data {
  TCPsocket socket;
  Uint32 timeout;
  int id;
  data(TCPsocket sock, Uint32 t, int i) : socket(sock), timeout(t), id(i) {}
};

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  SDLNet_Init();
  int curid = 0;
  int playernum = 0;
  SDL_Event event;
  IPaddress ip;
  SDLNet_ResolveHost(&ip, NULL, 1235);
  std::vector<data> socketvector;
  char tmp[1400];
  bool running = true;
  SDLNet_SocketSet sockets = SDLNet_AllocSocketSet(30);
  // SDL_Window *window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED,
  // SDL_WINDOWPOS_CENTERED, 600, 400, NULL); SDL_Renderer *renderer =
  // SDL_CreateRenderer(window, -1, 0);
  TCPsocket server = SDLNet_TCP_Open(&ip);
  char q[999];
  sqlite3 *db;
  sqlite3_stmt *stmt;

  if (sqlite3_open("test.db", &db) != SQLITE_OK) {
    fprintf(stderr, "Error opening database.\n");
    return 2;
  }

  while (running) {
    while (SDL_PollEvent(&event))
      if (event.type == SDL_QUIT ||
          event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
        running = false;
    TCPsocket tmpsocket = SDLNet_TCP_Accept(server);
    if (tmpsocket) {

      if (playernum < 30) {
        SDLNet_TCP_AddSocket(sockets, tmpsocket);
        socketvector.push_back(data(tmpsocket, SDL_GetTicks(), curid));
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
                char username[20];
                char password[20];
                player_data data;
                char *db_password = nullptr;

                sscanf(tmp, "1 %s %s \n", &username, &password);
                q[sizeof q - 1] = '\0';
                snprintf(q, sizeof q - 1,
                        "SELECT * FROM users WHERE username = '%s';", username);
                std::cout << q << std::endl;
                sqlite3_prepare(db, q, sizeof q, &stmt, NULL);
                bool done = false;
                while (!done) {
                  // printf("In select while\n");
                  switch (sqlite3_step(stmt)) {
                  case SQLITE_ROW:
                    db_password =
                        strdup((const char *)sqlite3_column_text(stmt, 2));
                    data.xpos = sqlite3_column_int(stmt, 3);
                    data.ypos = sqlite3_column_int(stmt, 4);
                    data.map_id = sqlite3_column_int(stmt, 5);
                    break;

                  case SQLITE_DONE:
                    done = true;
                    break;

                  default:
                    fprintf(stderr, "Failed.\n");
                    return 1;
                  }
                }
                if (db_password) {
                  if (strcmp(db_password, password) == 0) {
                    sprintf(tmp, "1 0 %d %d %d \n", data.map_id, data.xpos,
                            data.ypos);
                    std::cout << "Found user" << std::endl;
                    NetworkHelper::SendMessage(tmp, socketvector[i].socket);
                  } else {
                    sprintf(tmp, "1 1 \n");
                    NetworkHelper::SendMessage(tmp, socketvector[i].socket);
                  }
                } else {
                  sprintf(tmp, "1 1 \n");
                  NetworkHelper::SendMessage(tmp, socketvector[i].socket);
                }
                free(db_password);
                sqlite3_finalize(stmt);
                break;
              }
              case '2': {
                char username[20];
                char password[20];

                sscanf(tmp, "2 %s %s \n", &username, &password);
                q[sizeof q - 1] = '\0';
                snprintf(
                    q, sizeof q - 1,
                    "INSERT INTO users (username, password) VALUES ('%s','%s');",
                    username, password);
                std::cout << q << std::endl;
                sqlite3_prepare(db, q, sizeof q, &stmt, NULL);
                if (sqlite3_step(stmt) != SQLITE_DONE) {
                  sprintf(tmp, "2 1 \n");
                  NetworkHelper::SendMessage(tmp, socketvector[i].socket);
                } else {
                  sprintf(tmp, "2 0 \n");
                  NetworkHelper::SendMessage(tmp, socketvector[i].socket);
                }
                sqlite3_finalize(stmt);
                break;
              }
              case '3': {
                char username[20];
                player_data data;
                sscanf(tmp, "3 %s %d %d %d \n", &username, &data.map_id,
                      &data.xpos, &data.ypos);
                q[sizeof q - 1] = '\0';
                snprintf(q, sizeof q - 1,
                        "UPDATE users SET map_id='%d', x_position='%d', "
                        "y_position='%d' where username='%s';",
                        data.map_id, data.xpos, data.ypos, username);
                std::cout << q << std::endl;
                sqlite3_prepare(db, q, sizeof q, &stmt, NULL);
                if (sqlite3_step(stmt) != SQLITE_DONE) {
                  std::cout << "Saving player data failed" << std::endl;
                }
                sqlite3_finalize(stmt);
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
  // SDL_DestroyWindow(window);
  // SDL_DestroyRenderer(renderer);
  SDLNet_FreeSocketSet(sockets);
  SDLNet_TCP_Close(server);
  SDLNet_Quit();
  SDL_Quit();
}
