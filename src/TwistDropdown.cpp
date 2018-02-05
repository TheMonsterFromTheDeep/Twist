#include "Twist.h"

using Assets::Fonts::Body;
using Assets::Images::DropdownClosed;
using Assets::Images::DropdownOpen;

namespace Twist {
	void Dropdown::paint() {
		Vector bon = getBounds();

		float inset = bon.y / 4;
		float imgSize = Theme::FontSize;
		float imgCoord = (bon.y - imgSize) * 0.5f;

		if (open) {
			float menuHeight = bon.y * items.size();	
			GL::scissor(0, 0, bon.x, menuHeight);

			GL::beginStencil();
			GL::roundRectangle(0, 0, bon.x, menuHeight, inset);
			GL::endStencil();

			GL::rectangleV(0, 0, bon.x, menuHeight, Theme::DropdownTop, Theme::DropdownBottom);

			
			Body.setSize((int)Theme::FontSize);
			float y = bon.y;
			for (size_t i = 0; i < items.size(); ++i) {
				if (i != selectedIndex) {
					if (i == highlightedIndex) {
						GL::rectangleV(0, y, bon.x, bon.y, Theme::DropdownHighlightedTop, Theme::DropdownHighlightedBottom);
					}
					GL::color(Theme::DropdownFont);
					Body.render(items[i].c_str(), Vector(inset, y + inset));
					y += bon.y;
				}
			}
			if(selectedIndex == highlightedIndex)
				GL::rectangleV(0, 0, bon.x, bon.y, Theme::DropdownHighlightedTop, Theme::DropdownHighlightedBottom);
			GL::color(Theme::DropdownFont);
			Body.render(items[selectedIndex].c_str(), Vector(inset));

			GL::noStencil();

			DropdownOpen.draw(bon.x - imgSize - imgCoord, imgCoord, imgSize, imgSize);
		}
		else {
			if (highlighted)
				GL::rectangleV(0, 0, bon.x, bon.y, Theme::DropdownHighlightedTop, Theme::DropdownHighlightedBottom);
			else
				GL::rectangleV(0, 0, bon.x, bon.y, Theme::DropdownTop, Theme::DropdownBottom);

			GL::color(Theme::DropdownFont);
			Body.setSize((int)Theme::FontSize);
			Body.render(items[selectedIndex].c_str(), Vector(inset));

			DropdownClosed.draw(bon.x - imgSize - imgCoord, imgCoord, imgSize, imgSize);
		}
	}

	Vector Dropdown::getPreferredBounds() {
		Body.setSize((int)Theme::FontSize);
		float width = 0;
		for (auto s : items) {
			Vector fontBounds = Body.bounds(s.c_str());
			if (fontBounds.x > width) width = fontBounds.x;
		}
		float y = Theme::FontSize * 1.5f;
		return Vector(width + y * 0.5f + Theme::FontSize * 2, y);
	}

	void Dropdown::onMouseEnter() {
		highlighted = true;
	}

	void Dropdown::onMouseLeave() {
		highlighted = false;
	}

	void Dropdown::onMouseDown(MouseEvent& me) {
		if (open) {
			open = false;
			selectedIndex = highlightedIndex;
			releaseFocus();
		}
		else {
			open = true;
			highlightedIndex = selectedIndex;
			stealFocus();
		}
	}

	void Dropdown::onMouseMove(MouseEvent& me) {
		if (!open) return;

		Vector bon = getBounds();
		float height = bon.y * items.size();

		if (me.y < bon.y) {
			highlightedIndex = selectedIndex;
			return;
		}
		if (me.y > height) {
			int i = items.size() - 1;
			if (i == selectedIndex) --i;
			highlightedIndex = i;
			return;
		}
		
		float y = bon.y;
		for (size_t i = 0; i < items.size(); ++i) {
			if (i != selectedIndex) {
				if (me.y >= y && me.y < y + bon.y) {
					highlightedIndex = i;
					return;
				}
				y += bon.y;
			}
		}
	}
}