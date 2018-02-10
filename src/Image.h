#ifndef TWIST_IMAGE_H_
#define TWIST_IMAGE_H_

namespace Twist {
	class Image {
		unsigned int glId;

		float width_, height_;
	public:
		Image();

		float width();
		float height();

		void load(const char* path);
		void draw(float x, float y);
		void draw(float x, float y, float width, float height);

		void bind();
		void drawBound(float x, float y);
		void drawBound(float x, float y, float width, float height);
		void unbind();
	};
}

#endif