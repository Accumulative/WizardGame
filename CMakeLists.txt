cmake_minimum_required(VERSION 3.8)
project(Game2)
add_subdirectory("game_client" "game_client_output")
add_subdirectory("login_server" "login_server_output")
add_subdirectory("server" "game_server_output")
#"${PROJECT_SOURCE_DIR}/game_client_output"