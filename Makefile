
# GAME CLIENT
SRC_DIR := game_client/src
OBJ_DIR := game_client/obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/login_client/*.cpp) $(wildcard $(SRC_DIR)/ECS/*.cpp) $(wildcard shared/*.cpp) 
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
game: $(OBJ_FILES)
	g++ -std=c++11 -o output/game $^ -lSDL2 -lSDL2_net -lSDL2_ttf -lSDL2_image

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ -std=c++11 -c -o $@ $<

clean:
	rm -rf game_client/obj/**/*.o game_client/obj/*.o

# SERVER
SERVER_SRC_DIR := server/src
SERVER_OBJ_DIR := server/obj
SERVER_SRC_FILES := $(wildcard $(SERVER_SRC_DIR)/*.cpp) $(wildcard shared/*.cpp) 
SERVER_OBJ_FILES := $(patsubst $(SERVER_SRC_DIR)/%.cpp,$(SERVER_OBJ_DIR)/%.o,$(SERVER_SRC_FILES))

server: $(SERVER_OBJ_FILES)
	g++ -o output/server $^ -lSDL2 -lSDL2_net -lsqlite3

$(SERVER_OBJ_DIR)/%.o: $(SERVER_SRC_DIR)/%.cpp
	g++ -c -o $@ $<


# LOGIN SERVER
LOGIN_SRC_DIR := login_server/src
LOGIN_OBJ_DIR := login_server/obj
LOGIN_SRC_FILES := $(wildcard $(LOGIN_SRC_DIR)/*.cpp) $(wildcard shared/*.cpp) 
LOGIN_OBJ_FILES := $(patsubst $(LOGIN_SRC_DIR)/%.cpp,$(LOGIN_OBJ_DIR)/%.o,$(LOGIN_SRC_FILES))
login: $(LOGIN_OBJ_FILES)
	g++ -o output/login $^ -lSDL2 -lSDL2_net -lsqlite3

$(LOGIN_OBJ_DIR)/%.o: $(LOGIN_SRC_DIR)/%.cpp
	g++ -c -o $@ $<


# Unit tests
test: 
	g++ -std=c++11 -Wall -g tests/test.cpp shared/*.cpp -L/usr/include -l UnitTest++ -o tests/test.out
