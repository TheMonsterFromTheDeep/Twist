#include <SDL_opengl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include "Twist.h"

namespace Twist {
	static FT_Library ft;
	static GLuint font_texture;

	static void initializeFreeType() {
		static bool initialized = false;
		if (initialized) return;

		if (FT_Init_FreeType(&ft)) {
			throw std::runtime_error("Twist: Could not initialize FreeType.");
		}

		glGenTextures(1, &font_texture);
		glBindTexture(GL_TEXTURE_2D, font_texture);

		if (glGetError() != GL_NO_ERROR) {
			throw std::runtime_error("Twist: Could not setup OpenGL for font rendering. Perhaps assets were loaded before a window was active?");
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		initialized = true;
	}

	Font::Font() : ft_face(nullptr) { }

	void Font::load(const char *path) {
		if (ft_face) {
			throw std::logic_error("Twist: Trying to load already loaded font.");
		}

		initializeFreeType();

		if (FT_New_Face(ft, path, 0, &ft_face)) {
			throw std::invalid_argument("Twist: Could not load specified font.");
		}

		FT_Set_Pixel_Sizes(ft_face, 0, 24);
	}

	Font::~Font() {
		if(ft_face)
			FT_Done_Face(ft_face);
	}

	void Font::setSize(int pixels) {
		if (!ft_face) throw std::logic_error("Twist: Setting size of unloaded font.");

		FT_Set_Pixel_Sizes(ft_face, 0, pixels);
	}

	Vector Font::bounds(const char *text) {
		if (!ft_face) throw std::logic_error("Twist: Calculating bounds on unloaded font.");

		const char *p;

		FT_GlyphSlot g = ft_face->glyph;

		float x = 0;
		float y = 0;

		for (p = text; *p; ++p) {
			if (FT_Load_Char(ft_face, *p, FT_LOAD_RENDER))
				continue;
			x += g->advance.x * 0.015625f;
			if (g->bitmap.rows > y) y = (float)g->bitmap.rows;
		}

		return Vector(x, y);
	}

	void Font::render(const char *text, Vector pos) {
		render(text, pos.x, pos.y);
	}

	void Font::render(const char *text, float x, float y) {
		if (!ft_face) throw std::logic_error("Twist: Rendering text with unloaded font.");

		glBindTexture(GL_TEXTURE_2D, font_texture);

		const char *p;
		FT_GlyphSlot g = ft_face->glyph;

		for (p = text; *p; ++p) {
			if (FT_Load_Char(ft_face, *p, FT_LOAD_RENDER))
				continue;

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				g->bitmap.width,
				g->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				g->bitmap.buffer
			);

			float x2 = x + g->bitmap_left;
			float y2 = -y - g->bitmap_top;
			float w = (float)g->bitmap.width;
			float h = (float)g->bitmap.rows;

			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
				glVertex2f(x2, -y2);

				glTexCoord2f(1, 0);
				glVertex2f(x2 + w, -y2);

				glTexCoord2f(1, 1);
				glVertex2f(x2 + w, -y2 - h);

				glTexCoord2f(0, 1);
				glVertex2f(x2, -y2 - h);
			glEnd();

			x += g->advance.x * 0.015625f;
			y += g->advance.y * 0.015625f;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}