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

	Twist::Color TextFieldBackground(Twist::Color::gray(0.85f));
	Twist::Color TextFieldBackgroundInvalid(Twist::Color::rgbFromHex(0xff8989));
	Twist::Color TextFieldFont(Twist::Color::gray(0.05f));
	Twist::Color TextFieldCursor(Twist::Color::gray(0.2f));
	Twist::Color TextFieldSelection(Twist::Color::argbFromHex(0xaf2fa5dc));
	Twist::DpiX<float> TextFieldCursorWidth(1.f);

	Twist::Color DropdownTop(Twist::Color::gray(0.25f));
	Twist::Color DropdownBottom(Twist::Color::gray(0.2f));
	Twist::Color DropdownHighlightedTop(Twist::Color::rgbFromHex(0x6287c5));
	Twist::Color DropdownHighlightedBottom(Twist::Color::rgbFromHex(0x486eac));
	Twist::Color DropdownFont(Twist::Color::gray(0.9f));
	Twist::DpiX<float> DropdownDivisionWidth(0.5f);

	Twist::Color DividerColor(Twist::Color::gray(0.3f));
	Twist::Color DividerActiveColor(Twist::Color::gray(0.4f));
	Twist::DpiXY<float> DividerWidth(2.f);
	float DividerActivationMargin(1.05f);

	Twist::DpiXY<float> DividerSplitterWidth(10.f);

	Twist::DpiX<float> MenuBarMargin(2.f);
	Twist::Color MenuBarTop(Twist::Color::gray(0.47f));
	Twist::Color MenuBarBottom(Twist::Color::gray(0.43f));

	float CurveResolution(1.f);
}

namespace Assets {
	namespace Fonts {
		Twist::Font Body;
	}

	namespace Images {
		Twist::Image DividerTopRight;
		Twist::Image DividerBottomLeft;
		Twist::Image DropdownClosed;
		Twist::Image DropdownOpen;
	}
}