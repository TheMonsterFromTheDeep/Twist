#include "Twist.h"

namespace Twist {
	void VerticalDivider::layout() {
		if (children.size() < 1) return;

		if (children.size() == 1) {
			children[0]->updateLocation(Vector());
			children[0]->updateLayout(getSpace());
			return;
		}

		while (divisions.size() < children.size() - 1) {
			for (float& f : divisions) {
				f *= 0.9f;
			}
			divisions.push_back(0.95f);
		}

		Vector space = getSpace();
		Vector position(0, 0);
		Vector size(space.x, 0);
		size_t index = 0;
		for (auto &&w : children) {
			if (index < children.size() - 1)
				size.y = divisions[index] * space.y;
			else
				size.y = space.y - position.y;

			w->updateLayout(size);
			w->updateLocation(position);
			w->layout();

			position.y += size.y;
			++index;
		}
	}

	void VerticalDivider::paint() {
		Widget::paint();

		Vector space = getSpace();

		int division = 0;
		for (float d : divisions) {
			GL::color(Color::gray(division == activeDivision ? 0.4f : 0.3f));
			GL::rectangle(0, d * space.y - 1, space.x, 2);
			++division;
		}
	}

	void VerticalDivider::onMouseDown(MouseEvent& me) {
		if (activeDivision != -1 && me.button == MouseEvent::LEFT) {
			movingDivision = true;
			me.captured = true;
		}
	}

	void VerticalDivider::onMouseUp(MouseEvent& me) {
		if (me.button == MouseEvent::LEFT) {
			movingDivision = false;
		}
	}

	void VerticalDivider::onMouseMove(MouseEvent& me) {
		Vector space = getSpace();
		if (space.y < 0.001f) return;

		if (movingDivision) {
			divisions[activeDivision] = Util::clamp<float>(me.y / space.y, 0, 1);
			if (activeDivision > 0) {
				divisions[activeDivision] = Util::clampLow(divisions[activeDivision], divisions[activeDivision - 1]);
			}
			if ((size_t)activeDivision < divisions.size() - 1) {
				divisions[activeDivision] = Util::clampHigh(divisions[activeDivision], divisions[activeDivision + 1]);
			}
			me.captured = true;
			Widget::requestLayout();
			return;
		}

		activeDivision = -1;
		int newDivision = 0;

		for (float d : divisions) {
			float y = d * space.y;
			if (std::abs(me.y - y) < 3) {
				activeDivision = newDivision;
				break;
			}
			++newDivision;
		}
	}
}