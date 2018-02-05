#include "Twist.h"

using Assets::Fonts::Body;

namespace Twist {
	void Dropdown::paint() {
		Vector bon = getBounds();

		float inset = bon.y / 4;

		if(highlighted)
			GL::rectangleV(0, 0, bon.x, bon.y, Theme::DropdownHighlightedTop, Theme::DropdownHighlightedBottom);
		else
			GL::rectangleV(0, 0, bon.x, bon.y, Theme::DropdownTop, Theme::DropdownBottom);

		GL::color(Theme::DropdownFont);
		Body.setSize((int)Theme::FontSize);
		Body.render(items[0].c_str(), Vector(inset));

		float imgSize = Theme::FontSize;
		float imgCoord = (bon.y - imgSize) * 0.5f;

		Assets::Images::Dropdown.draw(bon.x - imgSize - imgCoord, imgCoord, imgSize, imgSize);
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
}