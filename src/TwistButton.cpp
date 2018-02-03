#include "Twist.h"

using Assets::Fonts::Body;

namespace Twist {
	Button::Button(std::string text_) : text(text_) { }

	void Button::paint() {
		Vector bounds = getBounds();

		float radius = bounds.y / 4;
		
		if (press && highlight)
			GL::rectangleV(0, 0, bounds.x, bounds.y, Color::gray(0.6f), Color::gray(0.7f));
		else if (highlight)
			GL::rectangleV(0, 0, bounds.x, bounds.y, Color::gray(0.9f), Color::gray(0.7f));
		else
			GL::rectangleV(0, 0, bounds.x, bounds.y, Color::gray(0.8f), Color::gray(0.7f));		

		GL::color(Color::gray(0.1f));
		Body.setSize((int)Theme::FontSize);
		Vector fontBounds = Body.bounds(text.c_str());
		Vector textLocation = Vector(radius, ((bounds - fontBounds) * 0.5f).y);
		Body.render(text.c_str(), textLocation);
	}

	Vector Button::getBounds() const {
		return Vector(getSpace().x, Theme::FontSize * 1.5f);
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