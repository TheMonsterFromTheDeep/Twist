#ifndef TWIST_TEXTFIELD_H_
#define TWIST_TEXTFIELD_H_

#include "Widget.h"

namespace Twist {
	class TextField : public Widget {
		std::wstring text;

		int cursorPosition = 0;
		int selectionPosition = 0;
		bool hasSelection = false;
		float pan = 0;

		int lowSelectedIndex();
		int highSelectedIndex();
		int selectionLength();

		int visualIndex(float x);
		
		bool readingMouse = false;

		void putString(const std::wstring&);
		void setCursorPosition(int);
	public:
		void onText(TextEvent&);
		void paint();
		Vector getPreferredBounds();

		void onFocus();
		void onUnfocus();

		void onKeyDown(KeyEvent&);

		void onMouseDown(MouseEvent&);
		void onMouseMove(MouseEvent&);
		void onMouseUp(MouseEvent&);
	};
}

#endif