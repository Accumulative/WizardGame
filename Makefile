SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/login_client/*.cpp) $(wildcard $(SRC_DIR)/ECS/*.cpp) 
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

game: $(OBJ_FILES)
	g++ -std=c++11 -o $@ $^ -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_net

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -std=c++11 -c -o $@ $<

server: 
	g++ src/server/*.cpp src/Vector2D.cpp -lSDL2 -lSDL2_net -lsqlite3 -o server

serverclean:
	rm server

clean:
	rm -rf obj/*.o obj/ECS/*.o

loginserver:
	g++ src/login_server/main.cpp -lSDL2 -lSDL2_net -lsqlite3 -o loginserver

loginserverclean:
	rm loginserver