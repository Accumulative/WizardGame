#include "Game.h"
#include "login_client/Login.h"

int main(int argc, char *argv[])
{
	SDL_Window * window;
	SDL_Renderer * renderer;
	const int FPS = 60;
	const int frameDelay = 1000 / FPS;
	Uint32 frameStart;
	int frameTime;
	bool fullscreen = false;


	int flags = 0;
	
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow("My game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
	
		auto * game = new Game(renderer);
		auto * login = new Login(game);
		while(game->state == GameState::LoggedOut) {
			frameStart = SDL_GetTicks();
			SDL_RenderClear(renderer);
			login->render();
			login->handleEvents();
			login->update();
			SDL_RenderPresent(renderer);
			frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime)
			{
				SDL_Delay(frameDelay - frameTime);
			}
		}

		game->init();
		while (game->state != GameState::NotRunning)
		{

			frameStart = SDL_GetTicks();
			if(game->state != GameState::NotConnected) {
			game->handleEvents();
				SDL_RenderClear(renderer);
				game->render();
				SDL_SetRenderDrawColor(renderer, 255,255,255,255);
				SDL_RenderPresent(renderer);
			}
			game->update();
			if(frameStart % (60 * 1000) < frameDelay) // About 50 seconds
				login->SavePlayerState(game->getPlayerState());
			frameTime = SDL_GetTicks() - frameStart;

			if (frameDelay > frameTime)
			{
				SDL_Delay(frameDelay - frameTime);
			}
		}
		login->SavePlayerState(game->getPlayerState()); // on logout or close game
	}
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}