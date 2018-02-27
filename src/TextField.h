#ifndef TWIST_TEXTFIELD_H_
#define TWIST_TEXTFIELD_H_

#include "Widget.h"

namespace Twist {
	class TextField : public Widget {
		std::wstring text;

		int cursorPosition = 0;
		float pan = 0;
	public:
		void onText(TextEvent&);
		void paint();
		Vector getPreferredBounds();

		void onFocus();
		void onUnfocus();

		void onKeyDown(KeyEvent&);

		void onMouseDown(MouseEvent&);
	};
}

#endif