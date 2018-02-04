#include "Twist.h"

using Assets::Fonts::Body;

namespace Twist {
	Button::Button(std::string text_) : text(text_) { }

	void Button::paint() {
		Vector bounds = getBounds();

		float radius = bounds.y / 4;
		
		if (press && highlight)
			GL::rectangleV(0, 0, bounds.x, bounds.y, Theme::ButtonPressedTop, Theme::ButtonPressedBottom);
		else if (highlight)
			GL::rectangleV(0, 0, bounds.x, bounds.y, Theme::ButtonHighlightedTop, Theme::ButtonHighlightedBottom);
		else
			GL::rectangleV(0, 0, bounds.x, bounds.y, Theme::ButtonUnpressedTop, Theme::ButtonUnpressedBottom);		

		GL::color(Theme::ButtonFontColor);
		Body.setSize((int)Theme::FontSize);
		Vector fontBounds = Body.bounds(text.c_str());
		Vector textLocation = Vector(radius, ((bounds - fontBounds) * 0.5f).y);
		Body.render(text.c_str(), textLocation);
	}

	Vector Button::getPreferredBounds() {
		Body.setSize((int)Theme::FontSize);
		Vector fontBounds = Body.bounds(text.c_str());
		float y = Theme::FontSize * 1.5f;
		return Vector(fontBounds.x + y * 0.5f, y);
	}

	void Button::onMouseEnter() {
		highlight = true;
	}

	void Button::onMouseLeave() {
		highlight = false;
	}

	void Button::onMouseDown(MouseEvent& me) {
		if (me.button == MouseEvent::Left) {
			press = true;
			captureExternalMouseEvents = true;
		}
	}

	void Button::onMouseUp(MouseEvent& me) {
		if (me.button == MouseEvent::Left) {
			press = false;
			captureExternalMouseEvents = false;
		}
	}
}