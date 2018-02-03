#include <iostream>
#include <SDL.h>
#include "Twist.h"

extern "C" {
	extern int Twist_main_(int, char**);
}

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << "Twist: Error intializing SDL2: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* Reuse argc for exit status, because why not */
	argc = Twist_main_(argc, argv);

	SDL_Quit();

	return argc;
}