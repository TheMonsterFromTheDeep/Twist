#include <SDL.h>
#include <SDL_opengl.h>
#include "Twist.h"

Twist::Font Assets::Fonts::Body;

void registerBuiltinAssets() {
	static bool registered = false;
	if (registered) return;

	Assets::Fonts::Body.load("Yantramanav-Light.ttf");

	registered = true;
}

namespace Twist {
	MouseEvent::Button getButton(int sdl) {
		switch (sdl) {
		case SDL_BUTTON_LEFT:
			return MouseEvent::LEFT;
		case SDL_BUTTON_MIDDLE:
			return MouseEvent::MIDDLE;
		case SDL_BUTTON_RIGHT:
			return MouseEvent::RIGHT;
		}
		return MouseEvent::NONE;
	}

	float invertY(Window& win, int y) {
		return win.getSpace().y - y;
	}

	static bool needLayoutUpdate = true;

	void Widget::requestLayout() {
		needLayoutUpdate = true;
	}

	void start(Window& win) {
		SDL_Event evt;

		SDL_Window *active = win.sdl_win;

		registerBuiltinAssets();

		for(;;) {
			if (SDL_PollEvent(&evt)) {
				switch (evt.type) {
				case SDL_QUIT:
					return;
				case SDL_MOUSEMOTION: {
						MouseEvent me((float)evt.motion.x, invertY(win, evt.motion.y), MouseEvent::NONE);
						win.performMouseMove(me);
					}
					break;
				case SDL_MOUSEBUTTONDOWN: {
						MouseEvent me((float)evt.button.x, invertY(win, evt.button.y), getButton(evt.button.button));
						win.performMouseDown(me);
					}
					break;
				case SDL_MOUSEBUTTONUP: {
						MouseEvent me((float)evt.button.x, invertY(win, evt.button.y), getButton(evt.button.button));
						win.performMouseUp(me);
					}
					break;
				default: { }
				}

				glClearColor(0.5f, 0.5f, 0.5f, 1.f);
				glClear(GL_COLOR_BUFFER_BIT);

				if (needLayoutUpdate) {
					win.layout();
					needLayoutUpdate = false;
				}
				win.paint();

				SDL_GL_SwapWindow(active);
			}
		}
	}
}