#include "Twist.h"

namespace Twist {
	void Widget::paint() {
		for (auto &&w : children) {
			GL::attachWidget(*w);
			w->paint();
			GL::detachWidget(*w, *this);
		}
	}

	Vector Widget::getLocation() const {
		return location;
	}

	Vector Widget::getSpace() const {
		return space;
	}

	Vector Widget::getBounds() const {
		return space;
	}

	bool Widget::isLocal(Vector v) {
		Vector loc = getLocation();
		Vector bon = getBounds();
		return (v.x >= loc.x) && (v.x <= loc.x + bon.x) &&
			   (v.y >= loc.y) && (v.y <= loc.y + bon.y);
	}

	void Widget::updateLayout(Vector space_) {
		space = space_;
	}

	void Widget::updateLocation(Vector loc_) {
		location = loc_;
	}

	void Widget::performMouseDown(MouseEvent& me) {
		if (containsMouse || captureExternalMouseEvents) {
			onMouseDown(me);
		}
		if (me.captured) return;
		for (auto &&w : children) {
			MouseEvent ce = me.childEvent(*w);
			w->performMouseDown(ce);
		}
	}

	void Widget::performMouseUp(MouseEvent& me) {
		if (containsMouse || captureExternalMouseEvents) {
			onMouseUp(me);
		}
		if (me.captured) return;
		for (auto &&w : children) {
			MouseEvent ce = me.childEvent(*w);
			w->performMouseUp(ce);
		}
	}

	void Widget::onMouseDown(MouseEvent& me) { }
	void Widget::onMouseUp(MouseEvent& me) { }
	void Widget::onMouseMove(MouseEvent& me) { }

	void Widget::performMouseMove(MouseEvent& me) {
		onMouseMove(me);
		if (me.captured) return;
		for (auto &&w : children) {
			MouseEvent ce = me.childEvent(*w);
			if (w->isLocal(Vector(me.x, me.y))) {
				if (!w->containsMouse) {
					w->containsMouse = true;
					w->onMouseEnter();
				}
				w->performMouseMove(ce);
			}
			else {
				if (w->containsMouse) {
					w->containsMouse = false;
					w->onMouseLeave();
				}
			}
		}
	}

	void Widget::onMouseEnter() { }
	void Widget::onMouseLeave() { }

	void Widget::layout() {
		if (children.size() < 1) return;

		float xTotal = 0;
		float xAllowed = space.x;

		float y = 0;
		float yNew = children[0]->getBounds().y;

		for (auto &&w : children) {
			w->space = space;
			w->layout();
			float xNew = w->getBounds().x;
			if (xNew > xAllowed) {
				xTotal = 0;
				y += yNew + 5;
				w->location = Vector(0, y);
				y += w->getBounds().y + 5;
				yNew = 0;
			}
			else if (xTotal + xNew > xAllowed) {
				xTotal = 0;
				y += yNew + 5;
				w->location = Vector(0, y);
				xTotal += xNew + 5;
				yNew = w->getBounds().y;
			}
			else {
				w->location = Vector(xTotal, y);
				xTotal += xNew + 5;
				float yCheck = w->getBounds().y;
				if (yCheck > yNew) yNew = yCheck;
			}
		}
	}

	void Widget::addChild(std::unique_ptr<Widget> child) {
		children.push_back(std::move(child));
	}
}