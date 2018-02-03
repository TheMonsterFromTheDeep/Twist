#include "Twist.h"

namespace Twist {
	void VerticalDivider::layout(LayoutEngine& e) {
		if (children.size() < 1) return;

		if (children.size() == 1) {
			e.setLocation(0, Vector());
			e.setBounds(0, getBounds());
			return;
		}

		while (divisions.size() < children.size() - 1) {
			for (float& f : divisions) {
				f *= 0.9f;
			}
			divisions.push_back(0.95f);
		}

		Vector bounds = getBounds();
		Vector position(0, 0);
		Vector size(bounds.x, 0);
		size_t index = 0;
		for (size_t i = 0; i < e.elements(); ++i) {
			if (index < children.size() - 1)
				size.y = divisions[index] * bounds.y;
			else
				size.y = bounds.y - position.y;

			e.setBounds(i, size);
			e.setLocation(i, position);

			position.y += size.y;
			++index;
		}
	}

	void VerticalDivider::paint() {
		Widget::paint();

		Vector bounds = getBounds();

		int division = 0;
		for (float d : divisions) {
			GL::color(Color::gray(division == activeDivision ? 0.4f : 0.3f));
			GL::rectangle(0, d * bounds.y - 1, bounds.x, 2);
			++division;
		}
	}

	void VerticalDivider::onMouseDown(MouseEvent& me) {
		if (activeDivision != -1 && me.button == MouseEvent::Left) {
			movingDivision = true;
			me.captured = true;
		}
	}

	void VerticalDivider::onMouseUp(MouseEvent& me) {
		if (me.button == MouseEvent::Left) {
			movingDivision = false;
		}
	}

	void VerticalDivider::onMouseMove(MouseEvent& me) {
		Vector bounds = getBounds();
		if (bounds.y < 0.001f) return;

		if (movingDivision) {
			divisions[activeDivision] = Util::clamp<float>(me.y / bounds.y, 0, 1);
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
			float y = d * bounds.y;
			if (std::abs(me.y - y) < 3) {
				activeDivision = newDivision;
				break;
			}
			++newDivision;
		}
	}
}