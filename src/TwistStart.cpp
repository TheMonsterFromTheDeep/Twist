#include <SDL.h>
#include <SDL_opengl.h>
#include "Twist.h"

static void registerBuiltinAssets() {
	static bool registered = false;
	if (registered) return;

	Assets::Fonts::Body.load("font/Yantramanav-Light.ttf");

	Assets::Images::DividerBottomLeft.load("img/DividerBottomLeft.png");
	Assets::Images::DividerTopRight.load("img/DividerTopRight.png");
	Assets::Images::DropdownClosed.load("img/DropdownClosed.png");
	Assets::Images::DropdownOpen.load("img/DropdownOpen.png");

	registered = true;
}

namespace Twist {
	MouseEvent::Button getButton(int sdl) {
		switch (sdl) {
		case SDL_BUTTON_LEFT:
			return MouseEvent::Left;
		case SDL_BUTTON_MIDDLE:
			return MouseEvent::Middle;
		case SDL_BUTTON_RIGHT:
			return MouseEvent::Right;
		}
		return MouseEvent::None;
	}

	float invertY(Window& win, int y) {
		return win.getBounds().y - y;
	}

	static bool needLayoutUpdate = true;

	void Widget::requestLayout() {
		needLayoutUpdate = true;
	}

	void start(Window& win) {
		SDL_Event evt;

		SDL_Window *active = win.sdl_win;

		registerBuiltinAssets();
		win.performLayout();

		for(;;) {
			if (SDL_PollEvent(&evt)) {
				switch (evt.type) {
				case SDL_QUIT:
					return;
				case SDL_MOUSEMOTION: {
						MouseEvent me((float)evt.motion.x, invertY(win, evt.motion.y), MouseEvent::None);
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
				case SDL_WINDOWEVENT: {
						if (evt.window.event == SDL_WINDOWEVENT_RESIZED) {
							int width = evt.window.data1;
							int height = evt.window.data2;
							glViewport(0, 0, width, height);

							glMatrixMode(GL_PROJECTION);
							glLoadIdentity();
							glOrtho(0, width, 0, height, -10, 10);

							glMatrixMode(GL_MODELVIEW);
							win.bounds = Vector(width, height);

							Widget::requestLayout();
						}
					}
					break;
				default: { }
				}

				glDisable(GL_SCISSOR_TEST);
					glClearColor(0.5f, 0.5f, 0.5f, 1.f);
					glClear(GL_COLOR_BUFFER_BIT);
				glEnable(GL_SCISSOR_TEST);

				if (needLayoutUpdate) {
					win.performLayout();
					needLayoutUpdate = false;
				}
				GL::attachWidget(win);
				win.paint();
				GL::detachWidget();

				SDL_GL_SwapWindow(active);
			}
		}
	}
}