#include "Twist.h"

namespace Twist {
	void FieldGroup::layout() {
		float xSpace = getSpace().x / children.size();
		height = 0;
		for (auto &&w : children) {
			w->updateLayout(Vector(xSpace, getSpace().y));
			w->layout();
			
			float newY = w->getBounds().y;
			if (newY > height) height = newY;
		}

		height /= (1 - Theme::FieldGroupMargins);
		Vector space(xSpace, height);
		Vector position;

		for (auto &&w : children) {
			w->updateLayout(space);
			w->layout();
			w->updateLocation(position);
			position.x += w->getBounds().x;
		}
	}

	Vector FieldGroup::getBounds() {
		return Vector(getSpace().x, height);
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