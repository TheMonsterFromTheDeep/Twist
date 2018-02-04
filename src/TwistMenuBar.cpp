#include "Twist.h"

namespace Twist {
	
	Vector MenuBar::getPreferredBounds() {
		float height = 0;
		for (auto &&w : children) {
			Vector bounds = w->getPreferredBounds();
			if (bounds.x == Widget::Unbounded || bounds.y == Widget::Unbounded) {
				throw std::runtime_error("Twist: MenuBar cannot have unbounded children.");
			}

			if (bounds.y > height) height = bounds.y;
		}

		return Vector(Widget::Unbounded, height + 2 * Theme::MenuBarMargin);
	}

	void MenuBar::layout(LayoutEngine& e) {
		float x = Theme::MenuBarMargin + Theme::DividerSplitterWidth.x();
		float height = getPreferredBounds().y;

		for (size_t i = 0; i < e.elements(); ++i) {
			Vector bounds = e.getPreferredBounds(i);
			float y = (height - bounds.y) * 0.5f;
			e.setLocation(i, Vector(x, y));
			e.setBounds(i, bounds);
		}
	}

	void MenuBar::paint() {
		Vector bounds = getBounds();

		GL::rectangleV(0, 0, bounds.x, bounds.y, Theme::MenuBarTop, Theme::MenuBarBottom);

		Widget::paint();
	}
}