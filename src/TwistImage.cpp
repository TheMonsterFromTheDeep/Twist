#include <stb_image.h>
#include <SDL_opengl.h>
#include "Twist.h"

namespace Twist {
	Image::Image() : glId(0) { }

	void Image::load(const char *path) {
		if (glId) {
			throw std::runtime_error("Twist: Trying to load an already loaded image");
		}

		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(path, &width, &height, &channels, 4);

		width_ = (float)width;
		height_ = (float)height;

		glGenTextures(1, &glId);
		glBindTexture(GL_TEXTURE_2D, glId);

		if (glGetError() != GL_NO_ERROR) {
			throw std::runtime_error("Twist: Could not create OpenGL texture for image. Perhaps assets were loaded before a window was active?");
		}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		if (glGetError() != GL_NO_ERROR) {
			throw std::runtime_error("Twist: Could not load image data into OpenGL texture. Perhaps the image asset does not exist?");
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		stbi_image_free(data);
	}

	void Image::draw(float x, float y) {
		draw(x, y, width_, height_);
	}

	void Image::draw(float x, float y, float width, float height) {
		bind();
		drawBound(x, y, width, height);
		unbind();
	}

	void Image::bind() {
		glBindTexture(GL_TEXTURE_2D, glId);
	}

	void Image::unbind() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Image::drawBound(float x, float y) {
		drawBound(x, y, width_, height_);
	}

	void Image::drawBound(float x, float y, float width, float height) {
		GL::beginQuads();
		GL::tex(0, 0);
		GL::vert(x, y);

		GL::tex(0, 1);
		GL::vert(x, y + height);

		x += width;

		GL::tex(1, 1);
		GL::vert(x, y + height);

		GL::tex(1, 0);
		GL::vert(x, y);
		GL::end();
	}

	float Image::width() {
		return width_;
	}

	float Image::height() {
		return height_;
	}
}