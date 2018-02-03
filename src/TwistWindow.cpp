#include <stdexcept>
#include <SDL.h>
#include <SDL_opengl.h>
#include "Twist.h"

#include "Platform.h"

#ifdef WINDOWS
	#include <Windows.h>
	#include <ShellScalingApi.h>
#endif

namespace Twist {
	static float dpiScaleXFac, dpiScaleYFac;

	float getDpiScaleX() {
		return dpiScaleXFac;
	}

	float getDpiScaleY() {
		return dpiScaleYFac;
	}

	void setDPIAware() {
#ifdef WINDOWS
		SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
#endif
	}

	void getDPIScalingFactor(float& scale_x, float& scale_y) {
#ifdef WINDOWS
		auto dc = GetDC(nullptr);

		int standard_px_per_logical_inch = 72;

		scale_x = (float)GetDeviceCaps(dc, LOGPIXELSX) / standard_px_per_logical_inch;
		scale_y = (float)GetDeviceCaps(dc, LOGPIXELSY) / standard_px_per_logical_inch;

		ReleaseDC(nullptr, dc);
#else
		scale_x = scale_y = 1;
#endif
	}

	Window::Window(const char *title, size_t width, size_t height) {
		if (!SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1))
			SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

		setDPIAware();
		getDPIScalingFactor(dpiScaleXFac, dpiScaleYFac);

		width = (size_t)(dpiScaleXFac * width);
		height = (size_t)(dpiScaleYFac * height);

		sdl_win = SDL_CreateWindow(title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		if (!sdl_win) {
			throw std::runtime_error(SDL_GetError());
		}

		gl_context = SDL_GL_CreateContext(sdl_win);
		if (!gl_context) {
			SDL_DestroyWindow(sdl_win);
			throw std::runtime_error(SDL_GetError());
		}

		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, 0, height, -10, 10);

		glMatrixMode(GL_MODELVIEW);

		bounds = Vector((float)width, (float)height);

		captureExternalMouseEvents = true;
	}

	Window::~Window() {
		SDL_DestroyWindow(sdl_win);
	}
}