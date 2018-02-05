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
			None,
			Left,
			Middle,
			Right
		};

		MouseEvent(float x_, float y_, Button button_);

		float x, y;
		Button button;
		bool captured = false;

		MouseEvent childEvent(const Widget&) const;
	};

	class LayoutEngine;

	class WidgetTemp {
	public:
		std::unique_ptr<Widget> widget;
		int insertionIndex;

		WidgetTemp(std::unique_ptr<Widget> widget_, int insertionIndex_ = -1);
	};

	class Widget {
		Vector location;
		Vector bounds;

		bool containsMouse = false;

		friend class Window;
		friend class LayoutEngine;
		friend void start(Window&);

		void performMouseDown(MouseEvent&);
		void performMouseUp(MouseEvent&);
		void performMouseMove(MouseEvent&);
		void performLayout();

		std::vector<WidgetTemp> insertionBuffer;
		std::vector<size_t> deletionBuffer;

		Widget* parent = nullptr;
	protected:
		std::vector<std::unique_ptr<Widget>> children;
		
		bool captureExternalMouseEvents = false;
	public:
		Vector getLocation() const;

		virtual void paint();
		Vector getBounds() const;
		virtual Vector getPreferredBounds();

		virtual void layout(LayoutEngine&);

		void addChild(std::unique_ptr<Widget>);
		void insertChild(std::unique_ptr<Widget>, size_t index);
		void removeChild(size_t index);
		void replaceChild(size_t index, std::unique_ptr<Widget>);

		bool isLocal(Vector point);

		virtual void onMouseEnter();
		virtual void onMouseLeave();
		virtual void onMouseMove(MouseEvent&);
		virtual void onMouseDown(MouseEvent&);
		virtual void onMouseUp(MouseEvent&);

		static void requestLayout();
		static const float Unbounded;

		bool hasParent();
		Widget& getParent();

		void stealFocus();
		void releaseFocus();
	};

	class LayoutEngine {
		Widget& owner;
	public:
		LayoutEngine(Widget& owner_);

		size_t elements();
		Vector getPreferredBounds(size_t index);

		void setLocation(size_t index, Vector location);
		void setBounds(size_t index, Vector location);
	};

	class Divider : public Widget {
		std::vector<float> divisions;
		int activeDivision;
		bool movingDivision = false;

		int mergeChild = -1;
		int mergeDivision;
		bool shouldMerge = false;

		bool isHorizontal = false;
		void undivide(Widget&, std::unique_ptr<Widget> newChild);
	public:
		virtual void paint();
		virtual void layout(LayoutEngine&);

		virtual void onMouseMove(MouseEvent&);
		virtual void onMouseDown(MouseEvent&);
		virtual void onMouseUp(MouseEvent&);

		void addChild();

		void splitVertical(Widget& child, bool isTop);
		void splitHorizontal(Widget& child, bool isRight);
		void initVerticalMerge(Widget& child, bool isTop);
		void initHorizontalMerge(Widget& child, bool isRight);
	};

	class DividerChild : public Widget {
		enum Corner {
			None = -1,
			BottomLeft = 0,
			TopRight = 1
		};

		Corner activeCorner;

		bool active;
		Vector actionCenter;
	public:
		virtual void paint();

		virtual void onMouseMove(MouseEvent&);
		virtual void onMouseDown(MouseEvent&);
	};

	class FieldGroup : public Widget {
	public:
		enum LayoutMode {
			Proportional,
			Even
		};

		virtual void paint();
		virtual void layout(LayoutEngine&);

		virtual Vector getPreferredBounds();

		void setLayoutMode(LayoutMode);
	private:
		float height;
		LayoutMode layoutMode = Even;
	};

	class Button : public Widget {
		std::string text;
		bool highlight = false;
		bool press = false;
	public:
		Button(std::string text_);

		void paint();
		virtual Vector getPreferredBounds();

		void onMouseEnter();
		void onMouseLeave();
		void onMouseDown(MouseEvent&);
		void onMouseUp(MouseEvent&);
	};

	class MenuBar : public Widget {
	public:
		virtual Vector getPreferredBounds();
		virtual void layout(LayoutEngine&);
		virtual void paint();
	};

	class Dropdown : public Widget {
		bool highlighted = false;
		bool open = false;

		int selectedIndex = 0;
		int highlightedIndex;
	public:
		std::vector<std::string> items;

		void paint();
		virtual Vector getPreferredBounds();

		void onMouseEnter();
		void onMouseLeave();
		void onMouseDown(MouseEvent&);
		void onMouseMove(MouseEvent&);
	};

	class Color {
	public:
		float r, g, b, a;
		Color();
		Color(float r_, float g_, float b_, float a_=1);
		Color(const Color& other);

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
		void detachWidget();

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
		void enableStencil();

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

	template<class T>
	class DpiXY {
		T value;
	public:
		DpiXY() : value(0) { }
		DpiXY(T value_) : value(value_) { }

		void operator=(T newValue) {
			value = newValue;
		}

		T x() {
			return value * getDpiScaleX();
		}

		T y() {
			return value * getDpiScaleY();
		}

		T area() {
			return x() * y();
		}
	};

	void start(Window&);

	namespace Debugging {
		class DebugStream {
		public:
			DebugStream & operator<<(const char*);
			DebugStream& operator<<(const std::string&);
			DebugStream& operator<<(int);
			DebugStream& operator<<(float);
			DebugStream& operator<<(double);
			DebugStream& operator<<(char);
		};
	}

	extern Debugging::DebugStream Debug;
}

namespace Assets {
	namespace Fonts {
		extern Twist::Font Body;
	}

	namespace Images {
		extern Twist::Image DividerTopRight;
		extern Twist::Image DividerBottomLeft;
		extern Twist::Image DropdownClosed;
		extern Twist::Image DropdownOpen;
	}
}

namespace Theme {
	extern Twist::DpiY<float> FontSize;

	extern Twist::Color FieldGroupBorder;
	extern float FieldGroupMargins;

	extern Twist::Color ButtonUnpressedTop;
	extern Twist::Color ButtonUnpressedBottom;
	extern Twist::Color ButtonHighlightedTop;
	extern Twist::Color ButtonHighlightedBottom;
	extern Twist::Color ButtonPressedTop;
	extern Twist::Color ButtonPressedBottom;
	extern Twist::Color ButtonFontColor;

	extern Twist::Color DropdownTop;
	extern Twist::Color DropdownBottom;
	extern Twist::Color DropdownHighlightedTop;
	extern Twist::Color DropdownHighlightedBottom;
	extern Twist::Color DropdownFont;

	extern Twist::Color DividerColor;
	extern Twist::Color DividerActiveColor;
	extern Twist::DpiXY<float> DividerWidth;
	extern float DividerActivationMargin;

	extern Twist::DpiXY<float> DividerSplitterWidth;

	extern Twist::DpiX<float> MenuBarMargin;
	extern Twist::Color MenuBarTop;
	extern Twist::Color MenuBarBottom;

	extern float CurveResolution;
}

#ifndef main
	#define main Twist_main_
	extern "C" int Twist_main_(int, char**);
#endif

#endif