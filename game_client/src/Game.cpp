#include "Game.h"
#include "Map.h"
#include <map>
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>
#include "ECS/TransformComponent.h"
#include "ECS/SpriteComponent.h"
#include "ECS/BattleComponent.h"
#include "ECS/KeyboardController.h"
#include "ECS/ColliderComponent.h"
#include "ECS/TileComponent.h"
#include "ECS/NetworkComponent.h"
#include "ECS/EventComponent.h"
#include "ECS/UILabel.h"
#include "Menu.h"
#include "Battle.h"

Map* map;
Manager manager;
Menu<EventTypes> * mainMenu;
Menu<EventTypes> * hoverMenu;
Menu<EventTypes> * yesNoPopup;
Menu<EventTypes> * okPopup;
Battle* battle;
Vector2D mouse;

SDL_Renderer* Game::renderer = nullptr;
int recentlyChangedMap = 0;
SDL_Rect Game::camera = { 0,0, 2000,1160};
const int Game::SCALE = 1;
const int Game::TILE_SIZE = 32;
const int TILESETSIZEX = 16;
const int TILESETSIZEY = 44;

AssetManager* Game::assets = new AssetManager(&manager);

auto& player(manager.addEntity());
auto& label(manager.addEntity());
auto& hover(manager.addEntity());

Game::Game(SDL_Renderer *renderer)
{
	this->renderer = renderer;
	player.addComponent<TransformComponent>(515.0f, 709.0f, 44, 33, 1, 1);
	state = GameState::LoggedOut;
	if (TTF_Init() == -1)
	{
		std::cout << "Error : SDL_TTF" << std::endl;
	}
	assets->AddTexture("terrain", "assets/magecity.png");
	assets->AddTexture("player", "assets/WizardSheet.png");
    assets->AddTexture("NPC1", "assets/RedWizardSheet.png");
    assets->AddTexture("NPC2", "assets/GreenWizardSheet.png");
    assets->AddTexture("enemy", "assets/YellowWizardSheet.png");
	assets->AddTexture("projectile", "assets/proj.png");
	assets->AddTexture("menu", "assets/Menu.png");
	assets->AddTexture("menuButton", "assets/MenuButton.png");
	assets->AddTexture("hover", "assets/ColTex.png");
	assets->AddTexture("wizardone", "assets/WizardOne.png");
	assets->AddTexture("wizardtwo", "assets/WizardTwo.png");
	assets->AddTexture("playerbar", "assets/BattleInfoVar.png");
	assets->AddTexture("textbox", "assets/Textbox.png");
	assets->AddFont("arial", "assets/arial.ttf", 16);
}

Game::~Game()
{
	delete assets;
	if(isOnline) {
		delete net;
		delete map;
		delete battle;
		delete mainMenu;
		delete hoverMenu;
		delete yesNoPopup;
		delete okPopup;
	}
}

void Game::init()
{
	std::cout << "I am initiating the game" << std::endl;
	isOnline = true;
	char ip [20];
	std::string ipS = "127.0.0.1";
	if(isOnline) {
		//127.0.0.1
		strcpy(ip, ipS.c_str()); 
		net = new Network(ip);
	}

	hover.addComponent<TransformComponent>(Game::SCALE);
	hover.addComponent<SpriteComponent>("hover", false, true);
	hover.getComponent<SpriteComponent>().setShow(false);

	map = new Map("terrain", Game::SCALE, Game::TILE_SIZE);
	mainMenu = new Menu<EventTypes>( "menu", { 
		{ "Continue", EventTypes::CloseMenu },
		{ "Options", EventTypes::InGameOptions },
		{ "Quit", EventTypes::Quit }
	}, "", 1 );
	hoverMenu = new Menu<EventTypes>( "menu", { 
		{ "Fight", EventTypes::Fight },
		{ "Trade", EventTypes::Trade },
		{ "View stats", EventTypes::ViewStats },
		{ "Back to game", EventTypes::CloseMenu }
	}, "", 1);
	yesNoPopup = new Menu<EventTypes>( "menu", { 
		{ "Accept", EventTypes::Accept },
		{ "Declined", EventTypes::Decline }
	}, "", 1);
	okPopup = new Menu<EventTypes>("menu", { 
		{ "OK", EventTypes::CloseMenu }
	}, "", 1 );
	//ecs implementation

	player.addComponent<NetworkComponent>();
	battle = new Battle("hover", net);
	SDL_Color white = { 0, 0, 0, 255 };
	
	label.addComponent<UILabel>(10, 10, "Test String", "arial", white, 200, false);

	// assets->CreateProjectile(Vector2D(600, 600), Vector2D(2,0),200, 2, "projectile");
	// assets->CreateProjectile(Vector2D(600, 620), Vector2D(2, 0), 200, 2, "projectile");
	// assets->CreateProjectile(Vector2D(400, 600), Vector2D(2, 1), 200, 2, "projectile");
	// assets->CreateProjectile(Vector2D(600, 600), Vector2D(2, -1), 200, 2, "projectile");

}

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& colliders(manager.getGroup(Game::groupColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& events(manager.getGroup(Game::groupEvents));
auto& ui(manager.getGroup(Game::groupUI));
auto& npcs(manager.getGroup(Game::groupNPCs));

void Game::handleEvents()
{
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0) {
		mainMenu->PollEvents(event);
		hoverMenu->PollEvents(event);
		yesNoPopup->PollEvents(event);
		if(player.getComponent<BattleComponent>().GetCurrentState()== BattleState::InProgress) {
			battle->PollEvents(event);
		} else {
			if(player.hasComponent<KeyboardController>())
				player.getComponent<KeyboardController>().update(event);
			switch (event.type)
			{
			case SDL_QUIT :
				state = GameState::NotRunning;
				break;
			case SDL_MOUSEMOTION: 
				mouse.x = event.motion.x + camera.x; 
				mouse.y = event.motion.y + camera.y; 
				break; 
			case SDL_KEYDOWN :
					switch (event.key.keysym.sym)
					{
						case SDLK_TAB:
							mainMenu->LoadMenu();
							break;
						case SDLK_ESCAPE:
							Game::state = GameState::NotRunning;
							break;
					}
				break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_RIGHT) {
					if( hover.getComponent<SpriteComponent>().getTarget() != -1 ) {
						hoverMenu->LoadMenu();
					}
				}
				break;
			default:
				break;
			}
		}
	}
}

void Game::update()
{
	if(isOnline) {
		auto* playernet = &player.getComponent<NetworkComponent>();
		net->send(&player);
		net->recv(&manager, &player);
		if(state == GameState::Connected) {
			TransformComponent playerPos = player.getComponent<TransformComponent>();
			std::stringstream ss;
			ss << "Player position: " << playerPos.position;
			label.getComponent<UILabel>().SetLabelText(ss.str(), "arial");
			manager.refresh();
			manager.update();
			if(player.getComponent<BattleComponent>().GetCurrentState() != BattleState::InProgress) {
                CheckCollisions();
				camera.x = static_cast<int>(player.getComponent<TransformComponent>().position.x - 400);
				camera.y = static_cast<int>(player.getComponent<TransformComponent>().position.y - 320);

				if(player.getComponent<BattleComponent>().GetBattleRequest() != -1 && player.getComponent<BattleComponent>().GetCurrentState() == BattleState::RequestedOf) {
					if(!yesNoPopup->IsLoaded()) {
						yesNoPopup->SetText("A player has invited you to a battle. Do you accept?");
						yesNoPopup->LoadMenu();
					}
				}

				if(player.getComponent<BattleComponent>().GetCurrentState() == BattleState::Accepted) {
					int playerid = hover.getComponent<SpriteComponent>().getTarget();
					for(auto& enemy : players) {
						if(enemy->getComponent<NetworkComponent>().getId() == playerid) {
								std::cout << " start battle ground" << std::endl;
							battle->StartBattle(&player, enemy);
							player.getComponent<BattleComponent>().SetState(BattleState::InProgress);
							// hover.getComponent<SpriteComponent>().setTarget(-1);
							yesNoPopup->Remove();
							break;
						}
					}
				} else if (player.getComponent<BattleComponent>().GetCurrentState() == BattleState::Declined) {
					okPopup->SetText("Your battle request has been declined");
					okPopup->LoadMenu();
					player.getComponent<BattleComponent>().RequestBattle(-1);
					player.getComponent<BattleComponent>().SetState(BattleState::NotStarted);
				}
			} else {
				if(battle->Finished()) {
					player.getComponent<BattleComponent>().RequestBattle(-1);
					player.getComponent<BattleComponent>().SetState(BattleState::NotStarted);
				}
			}
		} else {
			std::cout << "Connecting" << std::endl;
			if(playernet->getId() != -1 && !playernet->isLoaded()) {
				MapData data = GetMapData(player.getComponent<TransformComponent>().mapId);
				map->LoadMap(data.mapId, data.xSize, data.ySize, TILESETSIZEX, TILESETSIZEY);
				net->SwitchMap(&player, data.mapId, player.getComponent<TransformComponent>().position);
				playernet->setLoaded(true);
				state = GameState::Connected;
			}
		}
		EventTypes selectedOption = mainMenu->GetSelectedOption() + hoverMenu->GetSelectedOption() + yesNoPopup->GetSelectedOption();
		switch(selectedOption) {
			case EventTypes::CloseMenu:
				mainMenu->Remove();
				hoverMenu->Remove();
				break;
			case EventTypes::InGameOptions:
				break;
			case EventTypes::ViewStats:
				state = GameState::NotRunning;
				break;
			case EventTypes::Fight:
				if(player.getComponent<BattleComponent>().GetCurrentState() == BattleState::NotStarted && hover.getComponent<SpriteComponent>().getTarget() != -1) {
					std::cout << " Send net " << std::endl;
					net->SendPlayerEvent(&player, hover.getComponent<SpriteComponent>().getTarget(), Event::Battle);
					player.getComponent<NetworkComponent>().setWaitingPlayer(true);
					player.getComponent<BattleComponent>().SetState(BattleState::Requested);
					hoverMenu->Remove();
				}
				break;
			case EventTypes::Accept:
				if(player.getComponent<BattleComponent>().GetCurrentState() == BattleState::RequestedOf) {
						int playerid = player.getComponent<BattleComponent>().GetBattleRequest();
						std::cout << "Attempting battle with " << playerid << std::endl;
						
						for(auto enemy : players) {
							if(enemy->getComponent<NetworkComponent>().getId() == playerid) {
								std::cout << " start battle ground" << std::endl;
								player.getComponent<BattleComponent>().SetState(BattleState::InProgress);
								battle->StartBattle(&player, enemy);
								net->SendPlayerEvent(&player, playerid, Event::Battle);
								yesNoPopup->Remove();
								break;
							}
						}
				}
				break;
			case EventTypes::Decline:
				if(player.getComponent<BattleComponent>().GetCurrentState() == BattleState::RequestedOf) {
					int playerid = player.getComponent<BattleComponent>().GetBattleRequest();
					yesNoPopup->Remove();
					net->SendPlayerEvent(&player, playerid, Event::Decline);
					player.getComponent<BattleComponent>().SetState(BattleState::NotStarted);
				}
				break;
		}
	}

	if (camera.x < -200)
		camera.x = -200;
	if (camera.y < -200)
		camera.y = -200;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

	if(recentlyChangedMap > 0) {
		recentlyChangedMap--;
	}
	
}

void Game::CheckCollisions() {
    SDL_Rect playerCol = player.getComponent<ColliderComponent>().collider;
    for(auto& npc : npcs) {
        if (Collision::AABB(npc->getComponent<ColliderComponent>().collider, playerCol,
                            npc->getComponent<TransformComponent>().GetTotalVel() - player.getComponent<TransformComponent>().GetTotalVel()))
        {
            npc->getComponent<TransformComponent>().Collide();
            player.getComponent<TransformComponent>().Collide();
        }
    }
    for (auto& c : colliders)
    {
        if(c->hasComponent<ColliderComponent>()) {
            ColliderComponent cCol = c->getComponent<ColliderComponent>();
            for(auto& npc : npcs) {
                if (Collision::AABB(npc->getComponent<ColliderComponent>().collider, cCol.collider,
                                    npc->getComponent<TransformComponent>().GetTotalVel()))
                {
                    if(cCol.tag == ColliderType::Terrain || cCol.tag == ColliderType::Player) {
                        npc->getComponent<TransformComponent>().Collide();
                    }
                }
            }
            if (Collision::AABB(playerCol, cCol.collider, player.getComponent<TransformComponent>().velocity))
            {
                if(cCol.tag != ColliderType::Event) {
                    player.getComponent<TransformComponent>().Collide();
                } else if(cCol.tag == ColliderType::Event && recentlyChangedMap == 0) {
                    recentlyChangedMap = 20;
                    EventComponent ev = c->getComponent<EventComponent>();
                    MapData md = GetMapId(ev.mapId, ev.position.x, ev.position.y);
                    player.getComponent<TransformComponent>().position = md.pos * Game::TILE_SIZE * Game::SCALE;
                    map->Remove();
                    manager.refresh();
                    player.getComponent<TransformComponent>().mapId = md.mapId;
                    map->LoadMap(md.mapId, md.xSize, md.ySize, TILESETSIZEX, TILESETSIZEY);
                    net->SwitchMap(&player, md.mapId, md.pos);
                    break;
                }
            }
            if(Collision::MouseAA(mouse, cCol.collider)) {
                if(cCol.tag == ColliderType::Player) {
                    hover.getComponent<TransformComponent>().position = c->getComponent<TransformComponent>().position;
                    if(!hover.getComponent<SpriteComponent>().getShow()) {
                        hover.getComponent<SpriteComponent>().setShow(true);
                        hover.getComponent<SpriteComponent>().setTarget(c->getComponent<NetworkComponent>().getId());
                    }
                }
            }
            else {
                hover.getComponent<SpriteComponent>().setShow(false);
            }
        }
    }
}

void Game::setPlayerState(player_data p_data) {
	player.getComponent<TransformComponent>().position.x = p_data.xpos;
	player.getComponent<TransformComponent>().position.y = p_data.ypos;
	player.getComponent<TransformComponent>().mapId = p_data.map_id;
}
player_data Game::getPlayerState() {
	player_data data;
	data.map_id = player.getComponent<TransformComponent>().mapId ;
	data.xpos = static_cast<int>(player.getComponent<TransformComponent>().position.x);
	data.ypos = static_cast<int>(player.getComponent<TransformComponent>().position.y);
	return data;
}

void Game::render()
{
    if(player.getComponent<BattleComponent>().GetCurrentState() != BattleState::InProgress) {
        for (auto& t : tiles)
        {
            if(t->getComponent<TileComponent>().layer != TileLayer::Front)
                t->draw();
        }
        for (auto& u : ui)
        {
            u->draw();
        }
        for (auto& n : npcs)
        {
            n->draw();
        }
        // for (auto& c : colliders)
        // {
        // 	c->draw();
        // }
        for (auto& p : players)
        {
            p->draw();
        }
        for (auto& p : projectiles)
        {
            p->draw();
        }
        for (auto& t : tiles)
        {
            if(t->getComponent<TileComponent>().layer == TileLayer::Front)
                t->draw();
        }
        label.draw();
        hover.draw();
        hoverMenu->DrawMenu();
        yesNoPopup->DrawMenu();
    } else {
        battle->draw();
    }
    mainMenu->DrawMenu();
}