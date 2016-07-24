#include "../include/engine.h"

void engine_quit() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}