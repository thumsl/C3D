#include "../include/engine.h"

void quit() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}