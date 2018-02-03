#include <iostream>
#include <SDL.h>
#include "Twist.h"

extern "C" {
	extern int Twist_main_(int, char**);
}

void registerBuiltinThemes() {
	using namespace Twist::Themes;
	using namespace Twist::Theme;

	setFloat(FontSize, 11);
	setFloat(BodyFontSize, [] (){
		return getFloat(FontSize) * dpiScaleX();
	});
	setFloat(FieldGroupMargins, 0.05f);
	setFloat(SampleResolution, 1.f);
}

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << "Twist: Error intializing SDL2: " << SDL_GetError() << std::endl;
		return 1;
	}

	registerBuiltinThemes();

	/* Reuse argc for exit status, because why not */
	argc = Twist_main_(argc, argv);

	SDL_Quit();

	return argc;
}