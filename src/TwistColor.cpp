#include "Twist.h"

namespace Twist {
	Color::Color() : Color(0, 0, 0, 1) { }
	Color::Color(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) { }

	/* HSV -> RGB conversion taken from
	* https://www.cs.rit.edu/~ncs/color/t_convert.html
	*/
	Color Color::fromHSV(float h, float s, float v, float a){
		Color out;

		out.a = a;

		int i;
		float f, p, q, t;
		if (s == 0) {
			// achromatic (grey)
			out.r = out.g = out.b = v;
			return out;
		}
		h /= 60;			// sector 0 to 5
		i = (int)std::floor(h);
		f = h - i;			// factorial part of h
		p = v * (1 - s);
		q = v * (1 - s * f);
		t = v * (1 - s * (1 - f));
		switch (i) {
		case 0:
			out.r = v;
			out.g = t;
			out.b = p;
			break;
		case 1:
			out.r = q;
			out.g = v;
			out.b = p;
			break;
		case 2:
			out.r = p;
			out.g = v;
			out.b = t;
			break;
		case 3:
			out.r = p;
			out.g = q;
			out.b = v;
			break;
		case 4:
			out.r = t;
			out.g = p;
			out.b = v;
			break;
		default:		// case 5:
			out.r = v;
			out.g = p;
			out.b = q;
			break;
		}

		return out;
	}

#define INVERSE_255 0.003921568627450980392157
#define COMP(var, bits) ((var >> bits) & 0xff)

	float Color::scaleByte(unsigned char byte) {
		return (float)(byte * INVERSE_255);
	}

	Color Color::argbFromHex(unsigned int hex) {
		return Color(
			scaleByte(COMP(hex, 16)),
			scaleByte(COMP(hex, 8)),
			scaleByte(COMP(hex, 0)),
			scaleByte(COMP(hex, 24))
		);
	}

	Color Color::rgbFromHex(unsigned int hex) {
		return Color(
			scaleByte(COMP(hex, 16)),
			scaleByte(COMP(hex, 8)),
			scaleByte(COMP(hex, 0))
		);
	}

	Color Color::gray(float g) {
		return Color(g, g, g);
	}
}