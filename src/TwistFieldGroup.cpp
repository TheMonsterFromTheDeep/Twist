#include "Twist.h"

namespace Twist {
	void FieldGroup::layout(LayoutEngine& e) {
		float width = getBounds().x / children.size();
		getPreferredBounds();

		Vector bounds(width, height);
		Vector location;

		for (size_t i = 0; i < e.elements(); ++i) {
			e.setBounds(i, bounds);
			e.setLocation(i, location);
			location.x += bounds.x;
		}
	}

	Vector FieldGroup::getPreferredBounds() {
		height = 0;
		for (auto &&w : children) {
			float newY = w->getPreferredBounds().y;
			if (newY > height && newY != Widget::Unbounded) height = newY;
		}

		height /= (1 - Theme::FieldGroupMargins);
		return Vector(Widget::Unbounded, height);
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