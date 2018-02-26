#include "Twist.h"

namespace Twist {
	void Divider::layout(LayoutEngine& e) {
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

		if (isHorizontal) {
			Vector bounds = getBounds();
			Vector location(0, 0);
			Vector size(0, bounds.y);

			for (size_t i = 0; i < e.elements(); ++i) {
				if (i < children.size() - 1)
					size.x = divisions[i] * bounds.x - location.x - Theme::DividerWidth.x() * 0.5f;
				else
					size.x = bounds.x - location.x;

				e.setBounds(i, size);
				e.setLocation(i, location);

				location.x += size.x + Theme::DividerWidth.x();
			}
		}
		else {
			Vector bounds = getBounds();
			Vector location(0, 0);
			Vector size(bounds.x, 0);

			for (size_t i = 0; i < e.elements(); ++i) {
				if (i < children.size() - 1)
					size.y = divisions[i] * bounds.y - location.y - Theme::DividerWidth.y() * 0.5f;
				else
					size.y = bounds.y - location.y;

				e.setBounds(i, size);
				e.setLocation(i, location);

				location.y += size.y + Theme::DividerWidth.y();
			}
		}
	}

	void Divider::paint() {
		Widget::paint();

		Vector bounds = getBounds();

		int division = 0;
		if (isHorizontal) {
			for (float d : divisions) {
				GL::color(division == activeDivision ? Theme::DividerActiveColor : Theme::DividerColor);
				GL::rectangle(d * bounds.x - Theme::DividerWidth.x() * 0.5f, 0, Theme::DividerWidth.x(), bounds.y);
				++division;
			}
		}
		else {
			for (float d : divisions) {
				GL::color(division == activeDivision ? Theme::DividerActiveColor : Theme::DividerColor);
				GL::rectangle(0, d * bounds.y - Theme::DividerWidth.y() * 0.5f, bounds.x, Theme::DividerWidth.y());
				++division;
			}
		}

		if (shouldMerge) {
			GL::color(Twist::Color(0.3f, 0.3f, 0.3f, 0.2f));
			Vector l = children[mergeChild]->getLocation();
			Vector b = children[mergeChild]->getBounds();
			GL::rectangle(l.x, l.y, b.x, b.y);
		}
	}

	void Divider::onMouseDown(MouseEvent& me) {
		if (activeDivision != -1 && me.button == MouseEvent::Left) {
			movingDivision = true;
			me.captured = true;
		}
	}

	void Divider::onMouseUp(MouseEvent& me) {
		if (me.button == MouseEvent::Left) {
			movingDivision = false;
		}
		if (shouldMerge) {
			if (children.size() == 2 && hasParent()) {
				Divider *parent = dynamic_cast<Divider*>(&getParent());
				if (parent) {
					/* If the child to replace is 0 (mergeChild == 0), we want to replace ourselves with child 1
					 * and vice versa.
					 * Simply subtracting mergeChild from 1 gives us this relationship.
					 */
					parent->undivide(*this, std::move(children[1 - mergeChild]));
					Widget::requestLayout();
					return;
				}
			}

			/* Normal merging happens when any of the above fails */
			removeChild(mergeChild);
			divisions.erase(divisions.begin() + mergeDivision);
		}
		mergeChild = -1;
		shouldMerge = false;
	}

	void Divider::undivide(Widget& child, std::unique_ptr<Widget> newChild) {
		size_t index = 0;
		for (auto &&w : children) {
			if (w.get() == &child) break;
			++index;
		}

		if (index == children.size()) return;

		replaceChild(index, std::move(newChild));
	}

	void Divider::onMouseMove(MouseEvent& me) {
		Vector bounds = getBounds();
		if (bounds.x < 0.001f || bounds.y < 0.001f) return;

		if (mergeChild > -1) {
			shouldMerge = children[mergeChild]->isLocal(Vector(me.x, me.y));
		}

		if (movingDivision) {
			if(isHorizontal)
				divisions[activeDivision] = Util::clamp<float>(me.x / bounds.x, 0, 1);
			else
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

		if (isHorizontal) {
			for (float d : divisions) {
				float x = d * bounds.x;
				if (std::abs(me.x - x) < Theme::DividerWidth.x() * Theme::DividerActivationMargin) {
					activeDivision = newDivision;
					break;
				}
				++newDivision;
			}
		}
		else {
			for (float d : divisions) {
				float y = d * bounds.y;
				if (std::abs(me.y - y) < Theme::DividerWidth.y() * Theme::DividerActivationMargin) {
					activeDivision = newDivision;
					break;
				}
				++newDivision;
			}
		}
	}

	void Divider::addChild() {
		auto dc = std::make_unique<DividerChild>();
		Widget::addChild(std::move(dc));
	}

	void Divider::initVerticalMerge(Widget& child, bool isTop) {
		if (!isHorizontal) {
			size_t index = 0;

			for (auto &&w : children) {
				if (w.get() == &child) {
					break;
				}

				++index;
			}

			if (index == 0 && !isTop) return;
			if (index == children.size() - 1 && isTop) return;

			mergeChild = isTop ? index + 1 : index - 1;
			mergeDivision = isTop ? index : index - 1;
			shouldMerge = true;
		}
	}

	void Divider::initHorizontalMerge(Widget& child, bool isRight) {
		if (isHorizontal) {
			size_t index = 0;

			for (auto &&w : children) {
				if (w.get() == &child) {
					break;
				}

				++index;
			}

			if (index == 0 && !isRight) return;
			if (index == children.size() - 1 && isRight) return;

			mergeChild = isRight ? index + 1 : index - 1;
			mergeDivision = isRight ? index : index - 1;
			shouldMerge = true;
		}
	}

	void Divider::splitHorizontal(Widget& child, bool isRight) {
		size_t index = 0;

		for (auto &&w : children) {
			if (w.get() == &child) {
				break;
			}

			++index;
		}

		if (index == children.size()) return;

		if (isHorizontal || children.size() < 2) {
			isHorizontal = true;
			auto dc = std::make_unique<DividerChild>();

			if (isRight) {
				insertChild(std::move(dc), index + 1);
				float newDivision;
				if (index == children.size() - 1) {
					newDivision = 1;
				}
				else {
					newDivision = divisions[index + 1];
				}
				divisions.insert(divisions.begin() + index, newDivision);
				activeDivision = index;
				movingDivision = true;
			}
			else {
				insertChild(std::move(dc), index);
				float newDivision;
				if (index == 0) {
					newDivision = 0;
				}
				else {
					newDivision = divisions[index];
				}
				divisions.insert(divisions.begin() + index, newDivision);
				activeDivision = index;
				movingDivision = true;
			}
		}
		else {
			auto div = std::make_unique<Divider>();
			div->isHorizontal = true;
			if (!isRight) div->addChild();
			div->Widget::addChild(std::move(children[index]));
			if(isRight) div->addChild();
			div->divisions.push_back(isRight ? 1.f : 0.f);
			div->activeDivision = 0;
			div->movingDivision = true;
			replaceChild(index, std::move(div));
		}
	}

	void Divider::splitVertical(Widget& child, bool isTop) {
		size_t index = 0;

		for (auto &&w : children) {
			if (w.get() == &child) {
				break;
			}

			++index;
		}

		if (!isHorizontal || children.size() < 2) {
			isHorizontal = false;

			if (index == children.size()) return;

			auto dc = std::make_unique<DividerChild>();

			if (isTop) {
				insertChild(std::move(dc), index + 1);
				float newDivision;
				if (index == children.size() - 1) {
					newDivision = 1;
				}
				else {
					newDivision = divisions[index + 1];
				}
				divisions.insert(divisions.begin() + index, newDivision);
				activeDivision = index;
				movingDivision = true;
			}
			else {
				insertChild(std::move(dc), index);
				float newDivision;
				if (index == 0) {
					newDivision = 0;
				}
				else {
					newDivision = divisions[index];
				}
				divisions.insert(divisions.begin() + index, newDivision);
				activeDivision = index;
				movingDivision = true;
			}
		}
		else {
			auto div = std::make_unique<Divider>();
			div->isHorizontal = false;
			if (!isTop) div->addChild();
			div->Widget::addChild(std::move(children[index]));
			if (isTop) div->addChild();
			div->divisions.push_back(isTop ? 1.f : 0.f);
			div->activeDivision = 0;
			div->movingDivision = true;
			replaceChild(index, std::move(div));
		}
	}
}