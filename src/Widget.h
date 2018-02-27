#ifndef TWIST_WIDGET_H_
#define TWIST_WIDGET_H_

#include "Include.h"

namespace Twist {

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

	class KeyEvent {
	public:
		int keycode;
	};

	class TextEvent {
	public:
		std::wstring text;

		static void beginInput();
		static void endInput();
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
		bool focused;

		friend class Window;
		friend class LayoutEngine;
		friend void start(Window&);

		void performMouseDown(MouseEvent&);
		void performMouseUp(MouseEvent&);
		void performMouseMove(MouseEvent&);
		void performKeyDown(KeyEvent&);
		void performKeyUp(KeyEvent&);
		void performText(TextEvent&);
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
		bool isFocused();

		virtual void onFocus();
		virtual void onUnfocus();

		virtual void onMouseEnter();
		virtual void onMouseLeave();
		virtual void onMouseMove(MouseEvent&);
		virtual void onMouseDown(MouseEvent&);
		virtual void onMouseUp(MouseEvent&);

		virtual void onKeyDown(KeyEvent&);
		virtual void onKeyUp(KeyEvent&);

		virtual void onText(TextEvent&);

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

}

#endif