#include "Twist.h"

namespace Twist {
	const float Widget::Unbounded = -1;

	void Widget::paint() {
		for (auto &&w : children) {
			GL::attachWidget(*w);
			w->paint();
			GL::detachWidget();
		}
	}

	void Widget::performLayout() {
		LayoutEngine engine(*this);
		layout(engine);

		for (auto &&w : children) {
			w->performLayout();
		}
	}

	Vector Widget::getLocation() const {
		return location;
	}

	Vector Widget::getBounds() const {
		return bounds;
	}

	Vector Widget::getPreferredBounds() {
		return Vector(Widget::Unbounded);
	}

	bool Widget::isLocal(Vector v) {
		Vector loc = getLocation();
		Vector bon = getBounds();
		return (v.x >= loc.x) && (v.x <= loc.x + bon.x) &&
			   (v.y >= loc.y) && (v.y <= loc.y + bon.y);
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

		for (auto &&w : children) {
			MouseEvent ce = me.childEvent(*w);

			if (w->isLocal(Vector(me.x, me.y))) {
				if (!w->containsMouse) {
					w->containsMouse = true;
					w->onMouseEnter();
				}
			}
			else {
				if (w->containsMouse) {
					w->containsMouse = false;
					w->onMouseLeave();
				}
			}

			if (!me.captured)
				w->performMouseMove(ce);
		}
	}

	void Widget::onMouseEnter() { }
	void Widget::onMouseLeave() { }

	void Widget::layout(LayoutEngine &e) {
		if (children.size() < 1) return;

		float xTotal = 0;

		for (size_t i = 0; i < e.elements(); ++i) {
			Vector pb = e.getPreferredBounds(i);

			/* Simply consume one page for unbounded bounds */
			if (pb.x == Widget::Unbounded) pb.x = bounds.x;
			if (pb.y == Widget::Unbounded) pb.y = bounds.y;

			e.setBounds(i, pb);
			e.setLocation(i, Vector(xTotal, 0));

			xTotal += pb.x;
		}
	}

	void Widget::addChild(std::unique_ptr<Widget> child) {
		children.push_back(std::move(child));
	}

	LayoutEngine::LayoutEngine(Widget& owner_) : owner(owner_) { }

	size_t LayoutEngine::elements() {
		return owner.children.size();
	}

	Vector LayoutEngine::getPreferredBounds(size_t i) {
		return owner.children[i]->getPreferredBounds();
	}

	void LayoutEngine::setBounds(size_t i, Vector bounds) {
		owner.children[i]->bounds = bounds;
	}

	void LayoutEngine::setLocation(size_t i, Vector location) {
		owner.children[i]->location = location;
	}
}