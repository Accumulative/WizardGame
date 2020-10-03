#include "Network.h"
#include "ECS/TransformComponent.h"
#include "ECS/SpriteComponent.h"
#include "ECS/ColliderComponent.h"
#include "ECS/NetworkComponent.h"
#include "ECS/BattleComponent.h"
#include "ECS/KeyboardController.h"
#include "ECS/NPCComponent.h"
#include "Spells.h"
#include "../../shared/NetworkHelper.h"
#include "../../shared/string_helper.h"

  Network::Network(const char* ipchar) {
    SDLNet_Init();
    IPaddress ip;
    waiter = 0;
    myId = -1;
    if(SDLNet_ResolveHost(&ip, ipchar, 1234) ==-1)
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
  Network::~Network() {
    sprintf(tmp, "2 %d \n", myId);
    NetworkHelper::SendMessage(tmp, connection);
    SDLNet_TCP_Close(connection);
    SDLNet_FreeSocketSet(server);
    SDLNet_Quit();
  }
  void Network::send(Entity* e) {
    waiter++;
    NetworkComponent network = e->getComponent<NetworkComponent>();
    if(network.isReady()) {
      if(waiter >= 3) {
        TransformComponent transform = e->getComponent<TransformComponent>();
        if((oldPos == transform.position) == false || waiter > 200) { 
          waiter = 0;
          oldPos = transform.position;
          sprintf(tmp, "1 %d %d %f %f %f %f \n", network.getId(), transform.mapId, transform.position.x, transform.position.y,
          transform.velocity.x, transform.velocity.y);
          NetworkHelper::SendMessage(tmp, connection);
        }
      }
    }
  }
  void Network::SwitchMap(Entity* e, int mapId, Vector2D newPos) {
    sprintf(tmp, "3 1 %d %d %d %d \n", e->getComponent<NetworkComponent>().getId(), mapId, static_cast<int>(newPos.x), static_cast<int>(newPos.y));
    std::cout << "Sending: " << tmp << std::endl;
    waitingForMapChange = true;
    NetworkHelper::SendMessage(tmp, connection);
  }
  void Network::SendPlayerEvent(Entity* e, int playerid, Event event, int arg1, int arg2, int arg3) {
    NetworkComponent network = e->getComponent<NetworkComponent>();
    int eventid = static_cast<int>(event);
    if(network.isReady()) {
      switch(event) {
        case Event::Battle:
        case Event::Decline:
          sprintf(tmp, "4 %d %d %d \n", eventid, playerid, network.getId());
          break;
        case Event::Attack:
          sprintf(tmp, "4 %d %d %d %d \n", eventid, playerid, network.getId(), arg1); // Here arg1 is the spell used
          break;
      }
      NetworkHelper::SendMessage(tmp, connection);
    }
  }
  void Network::recv(Manager* manager, Entity* e) {
        auto &enemies(manager->getGroup(Game::groupPlayers));
        while (SDLNet_CheckSockets(server, 0) > 0 && SDLNet_SocketReady(connection)) {
            int offset = 0;
            do {
                offset += SDLNet_TCP_Recv(connection, tmp + offset, 1400);
                if (offset <= 0)
                    return;
            } while (tmp[strlen(tmp) - 1] != '\n');
            int type, id;
            auto res = split(tmp, '|');
            for (const std::string& r : res) {
                sscanf(r.c_str(), "%d %d", &type, &id);
                if (type == 0) {
                    if (myId == -1) {
                        NetworkComponent *network = &(e->getComponent<NetworkComponent>());
                        sscanf(r.c_str(), "0 %d", &id);
                        e->addComponent<SpriteComponent>("player", true, true);
                        e->getComponent<SpriteComponent>().addAnimation(0, 3, "Idle", 300);
                        e->getComponent<SpriteComponent>().addAnimation(1, 3, "WalkForward", 300);
                        e->getComponent<SpriteComponent>().addAnimation(2, 4, "WalkSideward", 300);
                        e->getComponent<SpriteComponent>().addAnimation(3, 4, "WalkBackward", 300);
                        e->getComponent<SpriteComponent>().Play("Idle");
                        e->addComponent<BattleComponent>("Player " + std::to_string(id), "wizardone");
                        e->addComponent<KeyboardController>();
                        e->addComponent<ColliderComponent>(ColliderType::Player, 7 * Game::SCALE, 25 * Game::SCALE,
                                                           20 * Game::SCALE, 26 * Game::SCALE);
                        e->addGroup(Game::groupPlayers);
                        network->setId(id);
                        myId = id;
                        std::cout << "Joined network with id " << id << std::endl;
                    }
                } else if (type == 1) {
                    if (id != myId) {
                        int i;
                        for (i = 0; i < enemies.size(); i++) {
                            if (enemies[i]->getComponent<NetworkComponent>().getId() == id) {
                                TransformComponent *transform = &(enemies[i]->getComponent<TransformComponent>());
                                int tmp2, tmp3;
                                Vector2D vel;
                                sscanf(r.c_str(), "1 %d %d %f %f %f %f", &tmp2, &tmp3, &(transform->position.x),
                                       &(transform->position.y), &vel.x, &vel.y);
                                if (tmp3 == e->getComponent<TransformComponent>().mapId) {
                                    enemies[i]->getComponent<NetworkComponent>().setResponding(true);
                                    if (vel.x != 0) {
                                        enemies[i]->getComponent<SpriteComponent>().Play("WalkSideward");
                                        if (vel.x < 0) {
                                            enemies[i]->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_HORIZONTAL;
                                        } else {
                                            enemies[i]->getComponent<SpriteComponent>().spriteFlip = SDL_FLIP_NONE;
                                        }
                                    } else if (vel.y > 0) {
                                        enemies[i]->getComponent<SpriteComponent>().Play("WalkForward");
                                    } else if (vel.y < 0) {
                                        enemies[i]->getComponent<SpriteComponent>().Play("WalkBackward");
                                    } else {
                                        enemies[i]->getComponent<SpriteComponent>().Play("Idle");
                                    }
                                } else {
                                    enemies[i]->getComponent<NetworkComponent>().setResponding(false);
                                }
                                break;
                            }
                        }
                        if (i >= enemies.size()) {
                            int tmp1, tmp6;
                            float tmp3, tmp4, tmp5, tmp2;
                            sscanf(r.c_str(), "1 %d %d %f %f %f %f", &tmp1, &tmp6, &tmp2, &tmp3, &tmp4, &tmp5);
                            if (tmp6 == e->getComponent<TransformComponent>().mapId) {
                                auto &newEnemy(manager->addEntity());
                                std::cout << "Getting new enemy" << std::endl;
                                newEnemy.addComponent<TransformComponent>(tmp2, tmp3, 44, 33, 1, 1);
                                newEnemy.addComponent<SpriteComponent>("enemy", true, true);
                                newEnemy.getComponent<SpriteComponent>().addAnimation(0, 3, "Idle", 300);
                                newEnemy.getComponent<SpriteComponent>().addAnimation(1, 3, "WalkForward", 300);
                                newEnemy.getComponent<SpriteComponent>().addAnimation(2, 4, "WalkSideward", 300);
                                newEnemy.getComponent<SpriteComponent>().addAnimation(3, 4, "WalkBackward", 300);
                                newEnemy.getComponent<SpriteComponent>().Play("Idle");
                                newEnemy.addComponent<ColliderComponent>(ColliderType::Player, 7 * Game::SCALE, 25 * Game::SCALE,
                                                                         20 * Game::SCALE, 26 * Game::SCALE);
                                newEnemy.addComponent<NetworkComponent>(tmp1);
                                newEnemy.addComponent<BattleComponent>("Player " + std::to_string(tmp1), "wizardtwo");
                                newEnemy.addGroup(Game::groupColliders);
                                newEnemy.addGroup(Game::groupPlayers);
                            }
                        }
                    }
                } else if (type == 2) {
                    for (int i = 0; i < enemies.size(); i++) {
                        if (enemies[i]->getComponent<NetworkComponent>().getId() == id) {
                            std::cout << "Enemy " << id << " left the game " << std::endl;
                            enemies[i]->destroy();
                            enemies.erase(enemies.begin() + i);
                            break;
                        }
                    }
                } else if (type == 3) {
                    NPC_Data n;
                    int8_t type;
                    sscanf(r.c_str(), "3 %d", &type);
                    if (type == 2) {
                        std::cout << "Finished loading map" << std::endl;
                        waitingForMapChange = false;
                    } else {
                        float tmp1, tmp2;
                        if (type == 1) {
                            char tempName[20];
                            char tempImage[20];
                            sscanf(r.c_str(), "3 1 %d %f %f %s %s %d", &n.id, &tmp1, &tmp2, &tempName, &tempImage, &n.canFight);
                            n.pos.x = tmp1;
                            n.pos.y = tmp2;
                            n.name = tempName;
                            n.image_name = tempImage;
                        } else if (type == 0) {
                            sscanf(r.c_str(), "3 0 %d %f %f", &n.id, &tmp1, &tmp2);
                            n.pos.x = tmp1;
                            n.pos.y = tmp2;
                        }
                        if ((!waitingForMapChange && type == 0) || (waitingForMapChange && type == 1)) {
                            int i;
                            auto &npcs = manager->getGroup(Game::groupNPCs);
                            for (i = 0; i < npcs.size(); i++) {
                                if (npcs[i]->getComponent<NPCComponent>().getId() == n.id) {
                                    std::cout << "Updating npc: " << n.name << std::endl;
                                    npcs[i]->getComponent<NPCComponent>().setDest(n.pos);
                                    break;
                                }
                            }
                            if (i >= npcs.size()) {
                                auto &newNpc(manager->addEntity());
                                newNpc.addComponent<NPCComponent>(n);
                                newNpc.addGroup(Game::groupNPCs);
                            }
                        }
                    }
                } else if (type == 4) {
                    ReceiveEvent(r.c_str(), e);
                    for (int i = 0; i < enemies.size(); i++) {
                        if (enemies[i]->getComponent<NetworkComponent>().getId() == id) {
                            enemies[i]->getComponent<NetworkComponent>().setResponding(true);
                        }
                    }
                }
            }
            int playerId = e->getComponent<NetworkComponent>().getId();
            for (int i = 0; i < enemies.size(); i++) {
                NetworkComponent *enNet = &enemies[i]->getComponent<NetworkComponent>();
                if (enNet->getId() != playerId) {
                    enNet->updateTimeout();
                }
            }
        }
    }
    void Network::ReceiveEvent(const char message[1400], Entity* e) {
        int tmp1, tmp2, tmp3, tmp4;
        sscanf(message, "4 %d %d %d", &tmp1, &tmp2, &tmp3);
        std::cout << "Net event " << message << std::endl;
        switch (static_cast<Event>(tmp1)) {
            case Event::Battle:
                sscanf(message, "4 %d %d %d", &tmp1, &tmp2, &tmp3);
                std::cout << "Battle route " << e->getComponent<BattleComponent>().GetBattleRequest() << std::endl;
                switch (e->getComponent<BattleComponent>().GetCurrentState()) {
                    case BattleState::NotStarted:
                        e->getComponent<BattleComponent>().RequestBattle(tmp3);
                        e->getComponent<BattleComponent>().SetState(BattleState::RequestedOf);
                        std::cout << "Receiving battle req from " << tmp3 << std::endl;
                        break;
                    case BattleState::Requested:
                        std::cout << "Accepted battle req from " << tmp3 << std::endl;
                        e->getComponent<BattleComponent>().SetState(BattleState::Accepted);
                        e->getComponent<BattleComponent>().RequestBattle(tmp3);
                        e->getComponent<BattleComponent>().setTurn(true);
                        e->getComponent<NetworkComponent>().setWaitingPlayer(false);
                        break;
                }
                break;
            case Event::Attack:
                if (!e->getComponent<BattleComponent>().getTurn()) {
                    sscanf(message, "4 %d %d %d %d", &tmp1, &tmp2, &tmp3, &tmp4);
                    e->getComponent<BattleComponent>().dealDamage(Spells::getDamage(static_cast<Spell>(tmp4)));
                    e->getComponent<BattleComponent>().setTurn(true);
                }
                break;
            case Event::Decline:
                e->getComponent<NetworkComponent>().setWaitingPlayer(false);
                break;
        }
  }
