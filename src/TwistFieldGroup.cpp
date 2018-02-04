#include "Twist.h"

namespace Twist {
	void FieldGroup::layout(LayoutEngine& e) {
		Vector prefBounds = getPreferredBounds();
		Vector actualBounds = getBounds();

		Vector bounds(0, height);
		if (layoutMode == Even)
			bounds.x = prefBounds.x / children.size();

		Vector location;

		for (size_t i = 0; i < e.elements(); ++i) {
			if(layoutMode == Proportional)
				bounds.x = (e.getPreferredBounds(i).x / prefBounds.x) * actualBounds.x;
			e.setBounds(i, bounds);
			e.setLocation(i, location);
			location.x += bounds.x;
		}
	}

	void FieldGroup::setLayoutMode(LayoutMode mode) {
		if (layoutMode != mode) {
			layoutMode = mode;
			Widget::requestLayout();
		}
	}

	Vector FieldGroup::getPreferredBounds() {
		float width = 0;

		height = 0;
		for (auto &&w : children) {
			width += w->getPreferredBounds().x;
			float newY = w->getPreferredBounds().y;
			if (newY > height && newY != Widget::Unbounded) height = newY;
		}

		height /= (1 - Theme::FieldGroupMargins);
		return Vector(width, height);
	}

	void FieldGroup::paint() {
		Vector bounds = getBounds();

		float inset = height * Theme::FieldGroupMargins;
		float radius = height / 4;

		GL::color(Color::gray(0.3f));
		GL::roundRectangle(0, 0, bounds.x, bounds.y, radius);

		GL::beginStencil();

		GL::roundRectangle(
			inset,
			inset,
			bounds.x - 2 * inset,
			bounds.y - 2 * inset,
			(bounds.y - 2 * inset) / 4
		);

		GL::endStencil();

		Widget::paint();

		GL::noStencil();

		if (children.size() > 1) {
			GL::color(Color::gray(0.3f));
			Vector rectPos(children[0]->getBounds().x, 0);
			for (size_t i = 1; i < children.size(); ++i) {
				GL::rectangle(rectPos.x - inset / 2, rectPos.y, inset, height);
				rectPos.x += children[i]->getBounds().x;
			}
		}
	}
}