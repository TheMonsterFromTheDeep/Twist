#ifndef TWIST_H_
#define TWIST_H_

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <map>

#include "geometry.h"

namespace Twist {
	namespace Util {
		template<class T>
		T clamp(T value, T low, T high) {
			if (value < low) value = low;
			if (value > high) value = high;
			return value;
		}

		template<class T>
		T clampLow(T value, T low) {
			if (value < low) value = low;
			return value;
		}

		template<class T>
		T clampHigh(T value, T high) {
			if (value > high) value = high;
			return value;
		}

		template<class T>
		constexpr T Pi(T multiplier=1) {
			return (T)(multiplier * 3.141592653589793238462643383279502884);
		}
	}

	class Window;
	class Widget;

	class MouseEvent {
	public:
		enum Button {
			NONE,
			LEFT,
			MIDDLE,
			RIGHT
		};

		MouseEvent(float x_, float y_, Button button_);

		float x, y;
		Button button;
		bool captured = false;

		MouseEvent childEvent(const Widget&) const;
	};

	class Widget {
		Vector location;
		Vector space;

		bool containsMouse = false;
		

		friend class Window;
		friend void start(Window&);

		void performMouseDown(MouseEvent&);
		void performMouseUp(MouseEvent&);
		void performMouseMove(MouseEvent&);
	protected:
		std::vector<std::unique_ptr<Widget>> children;

		bool captureExternalMouseEvents = false;
	public:
		Vector getLocation() const;
		Vector getSpace() const;

		virtual void paint();
		virtual Vector getBounds() const;

		virtual void layout();
		void updateLayout(Vector);
		void updateLocation(Vector);

		void addChild(std::unique_ptr<Widget>);

		bool isLocal(Vector point);

		virtual void onMouseEnter();
		virtual void onMouseLeave();
		virtual void onMouseMove(MouseEvent&);
		virtual void onMouseDown(MouseEvent&);
		virtual void onMouseUp(MouseEvent&);

		static void requestLayout();
	};

	class VerticalDivider : public Widget {
		std::vector<float> divisions;
		int activeDivision;
		bool movingDivision = false;
	public:
		virtual void paint();
		virtual void layout();

		virtual void onMouseMove(MouseEvent&);
		virtual void onMouseDown(MouseEvent&);
		virtual void onMouseUp(MouseEvent&);
	};

	class FieldGroup : public Widget {
	private:
		float height;
	public:
		virtual void paint();
		virtual void layout();

		virtual Vector getBounds();
	};

	class Button : public Widget {
		std::string text;
		bool highlight = false;
		bool press = false;
	public:
		Button(std::string text_);

		void paint();
		Vector getBounds() const;

		void onMouseEnter();
		void onMouseLeave();
		void onMouseDown(MouseEvent&);
		void onMouseUp(MouseEvent&);
	};

	class Color {
	public:
		float r, g, b, a;
		Color();
		Color(float r_, float g_, float b_, float a_=1);

		static Color fromHSV(float h, float s, float v, float a = 1);
		static Color rgbFromHex(unsigned int hex);
		static Color argbFromHex(unsigned int hex);
		static Color gray(float g);

		static float scaleByte(unsigned char byte);
	};

	namespace GL {
		/* Function used to convert floating point numbers, consistently,
		 * to an integer value.
		 */
		int toInt(float f);

		/* Attach the specified Widget for GL rendering. Should be used
		 * when painting Widget children.
		 */
		void attachWidget(const Widget& child);
		
		/* Detaches a Widget for GL rendering, giving the parent a valid
		 * rendering context.
		 *
		 * The general use of this and attachWidget() are to paint all
		 * children widgets. The process is to attach a child, paint it,
		 * detach it, and repeat.
		 */
		void detachWidget(const Widget& child, const Widget& parent);

		/* Sets the scissor test to an area within the current drawing context. 
		 * It should be noted that this does not respect context transform.
		 * (i.e. scale and rotation)
		 */
		void scissor(float x, float y, float width, float height);

		/* Resets the scissor area. */
		void descissor();

		void translate(float x, float y);
		void rotate(float amount);
		void scale(float x, float y);

		/* Resets the transform and scissor area. */
		void reset();

		/* Begins stencil */
		void beginStencil();
		/* Ends stencil */
		void endStencil();
		/* Disables stencil */
		void noStencil();

		void color(Color c);

		void beginPoints();
		void beginLines();
		void beginLineStrip();
		void beginLineLoop();
		void beginQuads();
		void beginQuadStrip();
		void beginTriangles();
		void beginTriangleStrip();
		void beginTriangleFan();
		void beginPolygon();
		void vert(float x, float y);
		void tex(float x, float y);
		void end();

		void rectangle(float x, float y, float width, float height);
		void rectangleV(float x, float y, float width, float height, Color top, Color bottom);
		void roundRectangle(float x, float y, float width, float height, float radius);
	};

	class Font {
		struct FT_FaceRec_ *ft_face;

	public:
		Font();
		~Font();

		void load(const char*);

		void setSize(int pixels);

		Vector bounds(const char* string);

		void render(const char* string, float x, float y);
		void render(const char* string, Vector position);
	};

	class Window : public Widget {
		struct SDL_Window *sdl_win;
		void *gl_context;

	public:
		Window(const char *title, size_t width, size_t height);
		~Window();

		friend void start(Window&);
	};

	float getDpiScaleX();
	float getDpiScaleY();

	template<class T>
	class DpiX {
		T value;
	public:
		DpiX() : value(0) { }
		DpiX(T value_) : value(value_) { }

		void operator= (T newValue) {
			value = newValue;
		}
		
		operator T() {
			return value * getDpiScaleX();
		}
	};

	template<class T>
	class DpiY {
		T value;
	public:
		DpiY() : value(0) { }
		DpiY(T value_) : value(value_) { }

		void operator= (T newValue) {
			value = newValue;
		}

		operator T() {
			return value * getDpiScaleY();
		}
	};

	void start(Window&);

	namespace debugging {
		class debugstream {
		public:
			debugstream & operator<<(const char*);
			debugstream& operator<<(const std::string&);
			debugstream& operator<<(int);
			debugstream& operator<<(float);
			debugstream& operator<<(double);
			debugstream& operator<<(char);
		};
	}

	extern debugging::debugstream debug;
}

namespace Assets {
	namespace Fonts {
		extern Twist::Font Body;
	}
}

namespace Theme {
	extern Twist::DpiY<float> FontSize;
	extern float FieldGroupMargins;
	extern float CurveResolution;
}

#ifndef main
	#define main Twist_main_
	extern "C" int Twist_main_(int, char**);
#endif

#endif