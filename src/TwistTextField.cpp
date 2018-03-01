#include "Twist.h"
#include <SDL_clipboard.h>
#include <SDL_keycode.h>

#include <locale>
#include <codecvt>

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

			if (hasSelection) {
				Vector selectionBounds = Body.bounds(text.substr(0, selectionPosition).c_str());
				GL::color(Theme::TextFieldSelection);
				GL::rectangle(textX + indexBounds.x, textY, selectionBounds.x - indexBounds.x, Theme::FontSize);
			}
		}
	}

	void TextField::setCursorPosition(int newPosition) {
		cursorPosition = newPosition;
		if (cursorPosition < 0) cursorPosition = 0;
		if (cursorPosition > text.length()) cursorPosition = text.length();
	}

	int TextField::lowSelectedIndex() {
		return cursorPosition > selectionPosition ? selectionPosition : cursorPosition;
	}

	int TextField::highSelectedIndex() {
		return cursorPosition < selectionPosition ? selectionPosition : cursorPosition;
	}

	int TextField::selectionLength() {
		return cursorPosition > selectionPosition ? (cursorPosition - selectionPosition) : (selectionPosition - cursorPosition);
	}

	void TextField::putString(const std::wstring& put) {
		if (hasSelection) {
			text.replace(text.begin() + lowSelectedIndex(), text.begin() + highSelectedIndex(), put);
			onTextChanged();
			setCursorPosition(lowSelectedIndex() + put.length());
			hasSelection = false;
		}
		else {
			text.insert(cursorPosition, put);
			onTextChanged();
			cursorPosition += put.length();
		}
	}

	void TextField::onText(TextEvent& te) {
		if (isFocused()) {
			putString(te.text);
		}
	}

	/* Event that child classes can override */
	void TextField::onTextChanged() { }

	void TextField::onKeyDown(KeyEvent& ke) {
		if (ke.keycode == SDLK_BACKSPACE) {
			if (text.length() > 0) {
				if (cursorPosition > 0 && cursorPosition <= text.length()) {
					if (hasSelection) {
						text.erase(text.begin() + lowSelectedIndex(), text.begin() + highSelectedIndex());
						onTextChanged();
						setCursorPosition(lowSelectedIndex());
						hasSelection = false;
					}
					else {
						setCursorPosition(cursorPosition - 1);
						text.erase(text.begin() + cursorPosition);
						onTextChanged();
					}
				}
			}
		}

		if (ke.keycode == SDLK_LEFT) {
			if (hasSelection && !ke.shiftDown) {
				setCursorPosition(highSelectedIndex());
				hasSelection = false;
			}
			else {
				if (ke.shiftDown) {
					if (!hasSelection) selectionPosition = cursorPosition;
					hasSelection = true;
				}
				setCursorPosition(cursorPosition - 1);
			}
		}
		if (ke.keycode == SDLK_RIGHT) {
			if (hasSelection && !ke.shiftDown) {
				setCursorPosition(lowSelectedIndex());
				hasSelection = false;
			}
			else {
				if (ke.shiftDown) {
					if (!hasSelection) selectionPosition = cursorPosition;
					hasSelection = true;
				}
				setCursorPosition(cursorPosition + 1);
			}
		}

		if (ke.controlDown) {
			if (ke.keycode == SDLK_v) {
				if (SDL_HasClipboardText()) {
					char *clipboard = SDL_GetClipboardText();

					std::string source(clipboard);
					std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

					putString(converter.from_bytes(source));

					SDL_free(clipboard);
				}
			}
			if (ke.keycode == SDLK_c) {
				if (hasSelection) {
					std::wstring source(text.substr(lowSelectedIndex(), selectionLength()));
					std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

					SDL_SetClipboardText(converter.to_bytes(source).c_str());
				}
			}
		}

		if (ke.keycode == SDLK_RETURN) {
			unfocus();
		}

		if (selectionPosition < 0) selectionPosition = 0;
		if (selectionPosition > text.length()) selectionPosition = text.length();
		if (!hasSelection) selectionPosition = cursorPosition;
		if (selectionPosition == cursorPosition) { hasSelection = false; }
	}

	void TextField::onFocus() {
		TextEvent::beginInput();
	}

	void TextField::onUnfocus() {
		TextEvent::endInput();
	}

	int TextField::visualIndex(float x) {
		x -= pan;

		Vector bounds = getBounds();

		Body.setSize((int)Theme::FontSize);
		Vector fontBounds = Body.bounds(text.c_str());

		float textX = Theme::FontSize * 0.5f;
		if (std::abs(x - textX) < Theme::TextFieldCursorWidth * 4) {
			return 0;
		}

		for (size_t i = 0; i < text.length(); ++i) {
			Vector indexBounds = Body.bounds(text.substr(0, i).c_str());
			float threshold = textX + indexBounds.x;
			if (std::abs(x - threshold) < Theme::TextFieldCursorWidth * 4) {
				return i;
				break;
			}
		}

		return text.length();
	}

	void TextField::onMouseDown(MouseEvent& me) {
		setCursorPosition(visualIndex(me.x));

		selectionPosition = cursorPosition;
		hasSelection = false;

		readingMouse = true;
		captureExternalMouseEvents = true;
	}

	void TextField::onMouseUp(MouseEvent& me) {
		readingMouse = false;
		captureExternalMouseEvents = false;
	}

	void TextField::onMouseMove(MouseEvent& me) {
		if (!readingMouse) return;

		int otherPosition = visualIndex(me.x);
		if (otherPosition == cursorPosition) {
			hasSelection = false;
		}
		else {
			selectionPosition = otherPosition;
			hasSelection = true;
		}
	}

	std::wstring TextField::getText() {
		return text;
	}

	void NumberField::onUnfocus() {
		TextField::onUnfocus();

		try {
			value = std::stof(text);
		}
		catch (std::exception& e) {
			value = 0;
			text = L"0";
		}
	}
}