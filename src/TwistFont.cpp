#include <SDL_opengl.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

#include "Twist.h"

namespace Twist {
	static FT_Library ft;

	static void initializeFreeType() {
		static bool initialized = false;
		if (initialized) return;

		if (FT_Init_FreeType(&ft)) {
			throw std::runtime_error("Twist: Could not initialize FreeType.");
		}

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

		setSize(Theme::FontSize);
	}

	Font::~Font() {
		if(ft_face)
			FT_Done_Face(ft_face);
	}

	static Glyph emptyGlyph(0, 0, 0, 0, 0, 0, 0);

	Glyph::Glyph(unsigned int texture_, float advanceX_, float advanceY_, float width_, float height_, float x_, float y_) :
		texture(texture_), advanceX(advanceX_), advanceY(advanceY_), width(width_), height(height_), x(x_), y(y_) { }
	Glyph::Glyph() : Glyph(0, 0, 0, 0, 0, 0, 0) { }

	std::unordered_map<wchar_t, Glyph> &Font::getGlyphMap(int size) {
		auto mapLoc = glyphs.find(size);
		if (mapLoc == glyphs.end()) {
			glyphs[size] = std::unordered_map<wchar_t, Glyph>();
			return glyphs[size];
		}

		return mapLoc->second;
	}

	const Glyph& Font::lookupGlyph(wchar_t what) {
		if (!currentGlyphMap) {
			currentGlyphMap = &getGlyphMap(currentGlyphSize);
		}

		auto glyphLoc = currentGlyphMap->find(what);
		if (glyphLoc != currentGlyphMap->end()) {
			return glyphLoc->second;
		}

		FT_GlyphSlot g = ft_face->glyph;
		if (FT_Load_Char(ft_face, what, FT_LOAD_RENDER)) {
			/* This glyph cannot be drawn */
			(*currentGlyphMap)[what] = emptyGlyph;
			return emptyGlyph;
		}

		GLuint gltex;

		glGenTextures(1, &gltex);
		glBindTexture(GL_TEXTURE_2D, gltex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ONE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

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

		glBindTexture(GL_TEXTURE_2D, 0);

		(*currentGlyphMap)[what] = Glyph(gltex, g->advance.x * 0.015625f, g->advance.y * 0.015625f, g->bitmap.width, g->bitmap.rows, g->bitmap_left, g->bitmap_top);

		return (*currentGlyphMap)[what];
	}

	void Font::setSize(int pixels) {
		if (!ft_face) throw std::logic_error("Twist: Setting size of unloaded font.");
		
		if (currentGlyphSize != pixels) {
			FT_Set_Pixel_Sizes(ft_face, 0, pixels);
			currentGlyphMap = &getGlyphMap(pixels);
			currentGlyphSize = pixels;
		}
	}

	Vector Font::bounds(const wchar_t *text) {
		if (!ft_face) throw std::logic_error("Twist: Calculating bounds on unloaded font.");

		const wchar_t *p;

		float x = 0;
		float y = 0;

		for (p = text; *p; ++p) {
			auto g = lookupGlyph(*p);

			x += g.advanceX;
			if (g.height > y) y = g.height;
		}

		return Vector(x, y);
	}

	Vector Font::bounds(const char *text) {
		if (!ft_face) throw std::logic_error("Twist: Calculating bounds on unloaded font.");

		const char *p;

		float x = 0;
		float y = 0;

		for (p = text; *p; ++p) {
			auto g = lookupGlyph(*p);

			x += g.advanceX;
			if (g.height > y) y = g.height;
		}

		return Vector(x, y);
	}

	void Font::render(const wchar_t *text, Vector pos) {
		render(text, pos.x, pos.y);
	}

	void Font::render(const char *text, Vector pos) {
		render(text, pos.x, pos.y);
	}

	void Font::render(const wchar_t *text, float x, float y) {
		if (!ft_face) throw std::logic_error("Twist: Rendering text with unloaded font.");

		const wchar_t *p;

		for (p = text; *p; ++p) {
			auto g = lookupGlyph(*p);

			glBindTexture(GL_TEXTURE_2D, g.texture);

			float x2 = x + g.x;
			float y2 = -y - g.y;
			float w = g.width;
			float h = g.height;

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

			x += g.advanceX;
			y += g.advanceY;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Font::render(const char *text, float x, float y) {
		if (!ft_face) throw std::logic_error("Twist: Rendering text with unloaded font.");

		const char *p;

		for (p = text; *p; ++p) {
			auto g = lookupGlyph(*p);

			glBindTexture(GL_TEXTURE_2D, g.texture);

			float x2 = x + g.x;
			float y2 = -y - g.y;
			float w = g.width;
			float h = g.height;

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

			x += g.advanceX;
			y += g.advanceY;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}