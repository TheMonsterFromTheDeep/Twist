#include <map>

#include <SDL_opengl.h>

#include "Twist.h"

namespace Twist {
	namespace GL {
		static int cornerX = 0, cornerY = 0;
		static int boundX = 0, boundY = 0;

		static float scaleFactorX = 1;
		static float scaleFactorY = 1;

		int toInt(float f) {
			return (int)std::round(f);
		}

		void attachWidget(const Widget& child) {
			Vector loc = child.getLocation();
			cornerX += toInt(loc.x);
			cornerY += toInt(loc.y);

			Vector bounds = child.getBounds();
			boundX = toInt(bounds.x);
			boundY = toInt(bounds.y);

			reset();
		}

		void detachWidget(const Widget& child, const Widget& parent) {
			Vector loc = child.getLocation();
			cornerX -= toInt(loc.x);
			cornerY -= toInt(loc.y);

			Vector bounds = parent.getBounds();
			boundX = toInt(bounds.x);
			boundY = toInt(bounds.y);

			reset();
		}

		void scissor(float x, float y, float width, float height) {
			int xi = Util::clamp(cornerX + toInt(x), cornerX, cornerX + boundX);
			int yi = Util::clamp(cornerY + toInt(y), cornerY, cornerY + boundY);
			glScissor(xi, yi,
				Util::clamp(toInt(width ), xi - cornerX, boundX),
				Util::clamp(toInt(height), yi - cornerY, boundY)
			);
		}

		void descissor() {
			glScissor(cornerX, cornerY, boundX, boundY);
		}

		void translate(float x, float y) {
			glTranslatef(x, y, 0);
		}

		void rotate(float amount) {
			glRotatef(amount, 0, 0, 1);
		}

		void scale(float x, float y) {
			glScalef(x, y, 0);
			scaleFactorX *= x;
			scaleFactorY *= y;
		}

		void reset() {
			descissor();

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			scaleFactorX = 1;
			scaleFactorY = 1;

			translate((float)cornerX, (float)cornerY);
		}

		void beginStencil() {
			glEnable(GL_STENCIL_TEST);

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			glClear(GL_STENCIL_BUFFER_BIT);
		}

		void endStencil() {
			glStencilMask(0x0);
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glStencilFunc(GL_EQUAL, 1, 0xFF);
		}

		void noStencil() {
			glDisable(GL_STENCIL_TEST);
		}

		void color(Color c) {
			glColor4f(c.r, c.g, c.b, c.a);
		}

		void rectangle(float x, float y, float width, float height) {
			beginQuads();
				vert(x, y);
				vert(x, y + height);
				x += width;
				vert(x, y + height);
				vert(x, y);
			end();
		}

		void rectangleV(float x, float y, float width, float height, Color top, Color bottom) {
			beginQuads();
				color(bottom);
				vert(x, y);
				vert(x + width, y);

				y += height;

				color(top);
				vert(x + width, y);
				vert(x, y);
			end();
		}

		void roundRectangle(float x, float y, float width, float height, float radius) {
			float leftInset = x + radius;
			float rightInset = x + width - radius;
			float bottomInset = y + radius;
			float topInset = y + height - radius;

			beginQuads();
			    /* Middle rectangle */
				vert(leftInset,  y);
				vert(leftInset,  y + height);
				vert(rightInset, y + height);
				vert(rightInset, y);

				/* Left rectangle */
				vert(x, bottomInset);
				vert(x, topInset);
				vert(leftInset, topInset);
				vert(leftInset, bottomInset);

				/* Right rectangle */
				vert(rightInset, bottomInset);
				vert(rightInset, topInset);
				vert(x + width, topInset);
				vert(x + width, bottomInset);
			end();

			int steps = (int)std::ceil(Util::Pi<float>(2) * radius * 0.25f * Theme::CurveResolution);
			float circle = (Util::Pi<float>(2)) / (steps * 4);

			beginTriangleFan();
			vert(rightInset, topInset);
				for (int i = 0; i <= steps; ++i) {
					float t = circle * i;
					vert(rightInset + radius * cos(t), topInset + radius * sin(t));
				}
			end();

			beginTriangleFan();
				vert(leftInset, topInset);
				for (int i = steps; i <= 2 * steps; ++i) {
					float t = circle * i;
					vert(leftInset + radius * cos(t), topInset + radius * sin(t));
				}
			end();

			beginTriangleFan();
				vert(leftInset, bottomInset);
				for (int i = 2 * steps; i <= 3 * steps; ++i) {
					float t = circle * i;
					vert(leftInset + radius * cos(t), bottomInset + radius * sin(t));
				}
			end();

			beginTriangleFan();
				vert(rightInset, bottomInset);
				for (int i = 3 * steps; i <= 4 * steps; ++i) {
					float t = circle * i;
					vert(rightInset + radius * cos(t), bottomInset + radius * sin(t));
				}
			end();
		}

		void beginPoints()        { glBegin(GL_POINTS);         }
		void beginLines()         { glBegin(GL_LINES);          }
		void beginLineStrip()     { glBegin(GL_LINE_STRIP);     }
		void beginLineLoop()      { glBegin(GL_LINE_LOOP);      }
		void beginQuads()         { glBegin(GL_QUADS);          }
		void beginQuadStrip()     { glBegin(GL_QUAD_STRIP);     }
		void beginTriangles()     { glBegin(GL_TRIANGLES);      }
		void beginTriangleStrip() { glBegin(GL_TRIANGLE_STRIP); }
		void beginTriangleFan()   { glBegin(GL_TRIANGLE_FAN);   }
		void beginPolygon()       { glBegin(GL_POLYGON);        }

		void vert(float x, float y) { glVertex2f(x, y); }
		void tex(float x, float y) { glTexCoord2f(x, y); }

		void end() { glEnd(); }
	}
}