#include "Twist.h"

namespace Theme {
	Twist::DpiY<float> FontSize(11.f);

	Twist::Color FieldGroupBorder(Twist::Color::gray(0.3f));
	float FieldGroupMargins(0.05f);

	Twist::Color ButtonUnpressedTop(Twist::Color::gray(0.8f));
	Twist::Color ButtonUnpressedBottom(Twist::Color::gray(0.7f));
	Twist::Color ButtonHighlightedTop(Twist::Color::gray(0.9f));
	Twist::Color ButtonHighlightedBottom(Twist::Color::gray(0.7f));
	Twist::Color ButtonPressedTop(Twist::Color::gray(0.6f));
	Twist::Color ButtonPressedBottom(Twist::Color::gray(0.7f));
	Twist::Color ButtonFontColor(Twist::Color::gray(0.1f));

	Twist::Color DividerColor(Twist::Color::gray(0.3f));
	Twist::Color DividerActiveColor(Twist::Color::gray(0.4f));
	Twist::DpiXY<float> DividerWidth(2.f);
	float DividerActivationMargin(1.05f);

	Twist::DpiXY<float> DividerSplitterWidth(10.f);

	float CurveResolution(1.f);
}

namespace Assets {
	namespace Fonts {
		Twist::Font Body;
	}

	namespace Images {
		Twist::Image DividerTopRight;
		Twist::Image DividerBottomLeft;
	}
}