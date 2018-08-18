cmake_minimum_required(VERSION 3.8)
project(game_client)

set(CMAKE_CXX_STANDARD 11)

include_directories(../include)
include_directories(../include/SDL2)
include_directories(../shared)

set(SOURCE_FILES
        ../include/SDL2/SDL.h
        ../include/SDL2/SDL_image.h
        ../include/SDL2/SDL_net.h
        ../include/SDL2/SDL_ttf.h
        ../include/sqlite3.h
        ../shared/NetworkHelper.h
        ../shared/data_struct.h
        ../shared/string_helper.h
        ../shared/Vector2D.h
        ../shared/Vector2D.cpp
        src/ECS/Animation.h
        src/ECS/BattleComponent.h
        src/ECS/ColliderComponent.h
        src/ECS/ECS.cpp
        src/ECS/ECS.h
        src/ECS/EventComponent.h
        src/ECS/KeyboardController.h
        src/ECS/NetworkComponent.h
        src/ECS/NPCComponent.h
        src/ECS/ProjectileComponent.h
        src/ECS/SpriteComponent.h
        src/ECS/TileComponent.h
        src/ECS/TransformComponent.h
        src/ECS/UILabel.h
        src/login_client/Login.cpp
        src/login_client/Login.h
        src/login_client/login_struct.h
        src/login_client/LoginNetwork.cpp
        src/login_client/LoginNetwork.h
        src/AssetManager.cpp
        src/AssetManager.h
        src/Battle.cpp
        src/Battle.h
        src/Collision.cpp
        src/Collision.h
        src/Game.cpp
        src/Game.h
        src/main.cpp
        src/Map.cpp
        src/Map.h
        src/Menu.h
        src/Network.cpp
        src/Network.h
        src/Spells.h
        src/Textbox.cpp
        src/Textbox.h
        src/TextureManager.cpp
        src/TextureManager.h)

find_package(SDL2 REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net")
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/game_client_output)
add_executable(game_client ${SOURCE_FILES})

#src/login_server/data_struct.h
#src/login_server/main.cpp
#src/server/main.cpp
#src/server/npc.cpp
#src/server/npc.h
#src/server/string_helper.h