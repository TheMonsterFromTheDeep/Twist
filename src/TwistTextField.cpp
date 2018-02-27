#include "Twist.h"
#include <SDL_keycode.h>

using Assets::Fonts::Body;

namespace Twist {
	Vector TextField::getPreferredBounds() {
		Body.setSize((int)Theme::FontSize);
		Vector fontBounds = Body.bounds(text.c_str());
		float y = Theme::FontSize * 1.5f;
		/* Preferred bounds gives room for ~10 characters */
		return Vector(Theme::FontSize * 10 + y, y);
	}

	void TextField::paint() {
		Vector bounds = getBounds();

		Body.setSize((int)Theme::FontSize);
		Vector fontBounds = Body.bounds(text.c_str());

		float textX = Theme::FontSize * 0.5f;
		float textY = (bounds.y - Theme::FontSize) * 0.5f;

		float scissorBound = bounds.x - 2 * textX;

		Vector indexBounds = Body.bounds(text.substr(0, cursorPosition).c_str());
		if (indexBounds.x + pan > scissorBound) {
			pan = scissorBound - indexBounds.x - Theme::TextFieldCursorWidth;
		}
		if (indexBounds.x + pan < 0) {
			pan = 0 - indexBounds.x + Theme::TextFieldCursorWidth;
		}
		
		GL::color(Theme::TextFieldBackground);
		GL::rectangle(0, 0, bounds.x, bounds.y);

		GL::translate(pan, 0);

		GL::color(Theme::TextFieldFont);

		GL::scissor(textX, 0, scissorBound, bounds.y);

		Body.render(text.c_str(), textX, textY);

		if (isFocused()) {
			
			GL::color(Theme::TextFieldCursor);
			GL::rectangle(textX + indexBounds.x, textY, Theme::TextFieldCursorWidth, Theme::FontSize);
		}
	}

	void TextField::onText(TextEvent& te) {
		if (isFocused()) {
			text.insert(cursorPosition, te.text);
			cursorPosition += te.text.length();
		}
	}

	void TextField::onKeyDown(KeyEvent& ke) {
		if (ke.keycode == SDLK_BACKSPACE) {
			if (cursorPosition > 0 && cursorPosition <= text.length() && text.length() > 0) {
				--cursorPosition;
				text.erase(text.begin() + cursorPosition);
			}
		}

		if (ke.keycode == SDLK_LEFT) --cursorPosition;
		if (ke.keycode == SDLK_RIGHT) ++cursorPosition;
		if (cursorPosition < 0) cursorPosition = 0;
		if (cursorPosition > text.length()) cursorPosition = text.length();
	}

	void TextField::onFocus() {
		TextEvent::beginInput();
	}

	void TextField::onUnfocus() {
		TextEvent::endInput();
	}

	void TextField::onMouseDown(MouseEvent& me) {
		Vector bounds = getBounds();

		Body.setSize((int)Theme::FontSize);
		Vector fontBounds = Body.bounds(text.c_str());

		float textX = Theme::FontSize * 0.5f;
		if (std::abs(me.x - textX) < Theme::TextFieldCursorWidth * 4) {
			cursorPosition = 0;
			return;
		}

		for(size_t i = 0; i < text.length(); ++i) {
			Vector indexBounds = Body.bounds(text.substr(0, i).c_str());
			float threshold = textX + indexBounds.x;
			if (std::abs(me.x - threshold) < Theme::TextFieldCursorWidth * 4) {
				cursorPosition = i;
				break;
			}
		}

		Vector textBounds = Body.bounds(text.c_str());
		if (me.x > textX + textBounds.x) cursorPosition = text.length();
	}
}