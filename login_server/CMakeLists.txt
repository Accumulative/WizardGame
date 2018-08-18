cmake_minimum_required(VERSION 3.8)
project(login_server)

set(CMAKE_CXX_STANDARD 11)

include_directories(../include)
include_directories(../include/SDL2)
include_directories(../shared)

set(SOURCE_FILES
        ../include/SDL2/SDL.h
        ../include/SDL2/SDL_net.h
        ../include/sqlite3.h
        ../shared/NetworkHelper.h
        ../shared/data_struct.h
        ../shared/string_helper.h
        ../shared/Vector2D.h
        ../shared/Vector2D.cpp
        src/main.cpp)

find_package(SDL2 REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -L/usr/local/lib -lSDL2 -lSDL2_net -lsqlite3")
add_executable(login_server ${SOURCE_FILES})