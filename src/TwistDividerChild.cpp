#include "Twist.h"

using Assets::Images::DividerBottomLeft;
using Assets::Images::DividerTopRight;

namespace Twist {
	void DividerChild::paint() {
		float w = Theme::DividerSplitterWidth.x();
		float h = Theme::DividerSplitterWidth.y();
		Vector bounds = getBounds();

		GL::color((activeCorner == BottomLeft) ? Theme::DividerActiveColor : Theme::DividerColor);
		DividerBottomLeft.draw(0, 0, w, h);

		GL::color((activeCorner == TopRight) ? Theme::DividerActiveColor : Theme::DividerColor);
		DividerTopRight.draw(bounds.x - w, bounds.y - w, w, h);
	}

	void DividerChild::onMouseMove(MouseEvent& me) {
		if (active) {
			if (!hasParent()) return;

			Vector diff = Vector(me.x, me.y) - actionCenter;
			if (diff.lensqr() < Theme::DividerSplitterWidth.area()) {
				return;
			}

			/* left = 0, down = 1, right = 2, up = 3 */
			int quadrant;
			if (std::abs(diff.x) > std::abs(diff.y)) {
				quadrant = diff.x > 0 ? 2 : 0;
			}
			else {
				quadrant = diff.y > 0 ? 3 : 1;
			}

			Divider& parent = static_cast<Divider&>(getParent());
			if (activeCorner == BottomLeft) {
				switch(quadrant) {
				case 0:
					parent.initHorizontalMerge(*this, false);
					break;
				case 1:
					parent.initVerticalMerge(*this, false);
					break;
				case 2:
					parent.splitHorizontal(*this, false);
					break;
				case 3:
					parent.splitVertical(*this, false);
					break;
				}
			}
			else {
				switch(quadrant) {
				case 0:
					parent.splitHorizontal(*this, true);
					break;
				case 1:
					parent.splitVertical(*this, true);
					break;
				case 2:
					parent.initHorizontalMerge(*this, true);
					break;
				case 3:
					parent.initVerticalMerge(*this, true);
					break;
				}
			}

			active = false;
			me.captured = true;

			return;
		}

		activeCorner = None;

		float w = Theme::DividerSplitterWidth.x();
		float h = Theme::DividerSplitterWidth.y();
		Vector bounds = getBounds();

		if (me.x < w && me.y < h) {
			activeCorner = BottomLeft;
		}

		if (me.x > bounds.x - w && me.y > bounds.y - h) {
			activeCorner = TopRight;
		}
	}

	void DividerChild::onMouseDown(MouseEvent& me){
		if (activeCorner != None) {
			active = true;
			actionCenter = Vector(me.x, me.y);
		}
	}
}