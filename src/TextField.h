#ifndef TWIST_TEXTFIELD_H_
#define TWIST_TEXTFIELD_H_

#include "Widget.h"

namespace Twist {
	class TextField : public Widget {
		int cursorPosition = 0;
		int selectionPosition = 0;
		bool hasSelection = false;
		float pan = 0;

		int lowSelectedIndex();
		int highSelectedIndex();
		int selectionLength();

		void selectAll();

		int visualIndex(float x);
		
		bool readingMouse = false;

		void putString(const std::wstring&);
		void setCursorPosition(int);
	protected:
		std::wstring text;
		bool isValid = true;
	public:
		void onText(TextEvent&);
		void paint();
		Vector getPreferredBounds();

		virtual void onFocus();
		virtual void onUnfocus();

		virtual void onKeyDown(KeyEvent&);

		virtual void onMouseDown(MouseEvent&);
		virtual void onMouseMove(MouseEvent&);
		virtual void onMouseUp(MouseEvent&);

		virtual void onTextChanged();

		std::wstring getText();
	};

	class NumberField : public TextField {
		float value;
	public:
		void onTextChanged();
	};
}

#endif