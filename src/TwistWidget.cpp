#include "Twist.h"

#include <SDL.h>

namespace Twist {
	WidgetTemp::WidgetTemp(std::unique_ptr<Widget> widget_, int insertionIndex_) : widget(std::move(widget_)), insertionIndex(insertionIndex_) { }

	const float Widget::Unbounded = -1;
	static Widget* focusOwner = nullptr;

	void Widget::stealFocus() {
		focusOwner = this;
	}

	void Widget::releaseFocus() {
		focusOwner = nullptr;
	}

	void Widget::paint() {
		for (auto &&w : children) {
			GL::attachWidget(*w);
			w->paint();
			GL::detachWidget();
		}
	}

	void Widget::performLayout() {
		for (auto i : deletionBuffer) {
			children.erase(children.begin() + i);
		}
		deletionBuffer.clear();

		for (auto &iw : insertionBuffer) {
			iw.widget->parent = this;
			if (iw.insertionIndex < 0) {
				children.push_back(std::move(iw.widget));
			}
			else {
				children.insert(children.begin() + iw.insertionIndex, std::move(iw.widget));
			}
		}
		insertionBuffer.clear();

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

	void Widget::performKeyDown(KeyEvent& ke) {
		onKeyDown(ke);
		for (auto &&w : children) {
			KeyEvent copy(ke);
			w->performKeyDown(copy);
		}
	}

	void Widget::performKeyUp(KeyEvent& ke) {
		onKeyUp(ke);
		for (auto &&w : children) {
			KeyEvent copy(ke);
			w->performKeyUp(copy);
		}
	}

	void Widget::onKeyUp(KeyEvent& ke) { }
	void Widget::onKeyDown(KeyEvent& ke) { }

	void Widget::performText(TextEvent& te) {
		onText(te);
		for (auto &&w : children) {
			w->performText(te);
		}
	}

	void Widget::onText(TextEvent& te) { }

	void TextEvent::beginInput() {
		SDL_StartTextInput();
	}

	void TextEvent::endInput() {
		SDL_StopTextInput();
	}

	void Widget::performMouseDown(MouseEvent& me) {
		bool wasFocused = focused;
		focused = false;
		if (containsMouse || captureExternalMouseEvents || focusOwner) {
			if (!focusOwner || focusOwner == this) {
				onMouseDown(me);
			}
		}
		if (containsMouse) {
			focused = true;
			onFocus();
		}
		if (!focused && wasFocused) {
			onUnfocus();
		}
		if (me.captured) return;
		for (auto &&w : children) {
			MouseEvent ce = me.childEvent(*w);
			w->performMouseDown(ce);
		}
	}

	void Widget::performMouseUp(MouseEvent& me) {
		if (containsMouse || captureExternalMouseEvents || focusOwner) {
			if (!focusOwner || focusOwner == this) {
				onMouseUp(me);
			}
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

	void Widget::onFocus() { }
	void Widget::onUnfocus() { }

	bool Widget::isFocused() { return focused; }

	void Widget::performMouseMove(MouseEvent& me) {
		if (!focusOwner || focusOwner == this) {
			onMouseMove(me);
		}

		for (auto &&w : children) {
			MouseEvent ce = me.childEvent(*w);

			if (w->isLocal(Vector(me.x, me.y))) {
				if (!w->containsMouse) {
					w->containsMouse = true;
					if(!focusOwner || focusOwner == w.get()) w->onMouseEnter();
				}
			}
			else {
				if (w->containsMouse) {
					w->containsMouse = false;
					if (!focusOwner || focusOwner == w.get()) w->onMouseLeave();
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
		insertionBuffer.push_back(WidgetTemp(std::move(child)));
		requestLayout();
	}

	void Widget::insertChild(std::unique_ptr<Widget> child, size_t index) {
		insertionBuffer.push_back(WidgetTemp(std::move(child), index));
		requestLayout();
	}

	void Widget::replaceChild(size_t index, std::unique_ptr<Widget> child) {
		child->parent = this;
		children[index] = std::move(child);
	}

	void Widget::removeChild(size_t index) {
		for (auto i : deletionBuffer) {
			if (i < index) {
				--index;
			}
		}
		deletionBuffer.push_back(index);
		requestLayout();
	}

	bool Widget::hasParent() {
		return parent != nullptr;
	}

	Widget& Widget::getParent() {
		if (!hasParent()) {
			throw std::runtime_error("Twist: Trying to access nonexistent parent!");
		}

		return *parent;
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