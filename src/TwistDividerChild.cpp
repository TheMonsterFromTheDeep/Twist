#include "Twist.h"

using Assets::Images::DividerBottomLeft;
using Assets::Images::DividerTopRight;

namespace Twist {
	void DividerChild::paint() {
		float w = Theme::DividerSplitterWidth.x();
		float h = Theme::DividerSplitterWidth.y();

		GL::color(Theme::DividerColor);
		DividerBottomLeft.draw(0, 0, w, h);

		Vector bounds = getBounds();
		DividerTopRight.draw(bounds.x - w, bounds.y - w, w, h);
	}
}