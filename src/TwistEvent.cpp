#include "Twist.h"

namespace Twist {
	MouseEvent::MouseEvent(float x_, float y_, Button button_) : x(x_), y(y_), button(button_) { }

	MouseEvent MouseEvent::childEvent(const Widget& w) const {
		Vector loc = w.getLocation();
		return MouseEvent(x - loc.x, y - loc.y, button);
	}
}