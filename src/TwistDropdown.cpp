#include "Twist.h"

using Assets::Fonts::Body;
using Assets::Images::DropdownClosed;
using Assets::Images::DropdownOpen;

#define IMG_RATIO 1.2f

namespace Twist {
	DropdownItem::DropdownItem(std::string text_, Reference<Image> icon_) : text(text_), icon(icon_) { }

	DropdownItems::DropdownItems(std::string title_) : title(title_) { }

	float DropdownItems::width() {
		float width = 0;
		for (auto s : items) {
			Vector fontBounds = Body.bounds(s.text.c_str());
			if (s.icon) {
				fontBounds.x += Theme::FontSize * IMG_RATIO;
			}
			if (fontBounds.x > width) width = fontBounds.x;
		}
		return width;
	}

	void Dropdown::paint() {
		Vector bon = getBounds();

		float inset = bon.y / 4;
		float imgSize = Theme::FontSize;
		float imgCoord = (bon.y - imgSize) * 0.5f;

		if (highlighted && !open)
			GL::rectangleV(0, 0, bon.x, bon.y, Theme::DropdownHighlightedTop, Theme::DropdownHighlightedBottom);
		else
			GL::rectangleV(0, 0, bon.x, bon.y, Theme::DropdownTop, Theme::DropdownBottom);

		Vector closedItem(inset, inset);

		if (sections[selectedSection].items[selectedIndex].icon) {
			GL::color(Twist::Color(1, 1, 1));
			sections[selectedSection].items[selectedIndex].icon->draw(closedItem.x, imgCoord, Theme::FontSize, Theme::FontSize);
			closedItem.x += Theme::FontSize * IMG_RATIO;
		}
		GL::color(Theme::DropdownFont);
		Body.setSize((int)Theme::FontSize);
		Body.render(sections[selectedSection].items[selectedIndex].text.c_str(), closedItem);

		(open ? DropdownOpen : DropdownClosed).draw(bon.x - imgSize - imgCoord, imgCoord, imgSize, imgSize);

		if (open) {
			float menuHeight = 0;
			for (auto s : sections) {
				float h = bon.y * s.items.size();
				if (h > menuHeight) menuHeight = h;
			}

			GL::scissor(0, bon.y, bon.x * sections.size(), menuHeight);

			GL::beginStencil();
			GL::roundRectangle(0, bon.y, bon.x * sections.size(), menuHeight, inset);
			GL::endStencil();

			GL::rectangleV(0, bon.y, bon.x * sections.size(), menuHeight, Theme::DropdownTop, Theme::DropdownBottom);
			
			Body.setSize((int)Theme::FontSize);
			float x = 0;
			bool line = false;
			size_t currentSection = 0;
			for (auto s : sections) {
				float y = menuHeight; /* + bon.y - bon.y */
				for (size_t i = s.items.size(); i-- > 0;) {
					if (currentSection == highlightedSection && i == highlightedIndex) {
						GL::rectangleV(x, y, bon.x, bon.y, Theme::DropdownHighlightedTop, Theme::DropdownHighlightedBottom);
					}
					float rx = x + inset;
					if (s.items[i].icon) {
						GL::color(Twist::Color(1, 1, 1));
						s.items[i].icon->draw(rx, y + imgCoord, Theme::FontSize, Theme::FontSize);
						rx += Theme::FontSize * IMG_RATIO;
					}
					GL::color(Theme::DropdownFont);
					Body.render(s.items[i].text.c_str(), Vector(rx, y + inset));
					y -= bon.y;
				}
				if (line) {
					GL::color(Theme::DropdownFont);
					GL::rectangle(x - Theme::DropdownDivisionWidth / 2, bon.y + inset, Theme::DropdownDivisionWidth, menuHeight - (2 * inset));
				}
				line = true;
				x += bon.x;

				++currentSection;
			}

			GL::noStencil();
		}
	}

	Vector Dropdown::getPreferredBounds() {
		Body.setSize((int)Theme::FontSize);
		float width = 0;
		for (auto s : sections) {
			float w = s.width();
			if (w > width) width = w;
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
			selectedSection = highlightedSection;
			releaseFocus();
		}
		else {
			open = true;
			highlightedIndex = selectedIndex;
			highlightedSection = selectedSection;
			stealFocus();
		}
	}

	void Dropdown::onMouseMove(MouseEvent& me) {
		if (!open) return;

		Vector bon = getBounds();
		float height = 0;
		for (auto s : sections) {
			float h = bon.y * s.items.size();
			if (h > height) height = h;
		}

		if (me.x < 0) {
			highlightedSection = 0;
		}
		else if (me.x > bon.x * sections.size()) {
			highlightedSection = sections.size() - 1;
		}
		else {
			float x = 0;
			for (highlightedSection = 0; highlightedSection < sections.size(); ++highlightedSection) {
				if (me.x > x && me.x < x + bon.x) {
					break;
				}
				x += bon.x;
			}
		}

		size_t itemCount = sections[highlightedSection].items.size();
		float minY = bon.y + height - bon.y * itemCount;

		if (me.y < minY) {
			highlightedIndex = 0;
			return;
		}
		if (me.y > height) {
			highlightedIndex = itemCount - 1;
			return;
		}

		float y = minY;
		for (highlightedIndex = 0; highlightedIndex < itemCount; ++highlightedIndex) {
			if (me.y >= y && me.y < y + bon.y) {
				return;
			}
			y += bon.y;
		}
	}
}