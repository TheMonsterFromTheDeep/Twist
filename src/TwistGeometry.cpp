#include "geometry.h"
#include <cmath>

Vector::Vector() : x(0), y(0) { }
Vector::Vector(float dupli) : x(dupli), y(dupli) { }
Vector::Vector(float x_, float y_) : x(x_), y(y_) { }

Vector Vector::operator+(Vector v) const { return Vector(x + v.x, y + v.y); }
Vector Vector::operator-(Vector v) const { return Vector(x - v.x, y - v.y); }

void Vector::operator+=(Vector v) { x += v.x; y += v.y; }
void Vector::operator-=(Vector v) { x -= v.x; y -= v.y; }

Vector Vector::operator*(float s) const { return Vector(x * s, y * s); }
Vector Vector::operator/(float s) const { return Vector(x / s, y / s); }

void Vector::operator*=(float s) { x *= s; y *= s; }
void Vector::operator/=(float s) { x /= s; y /= s; }

float Vector::dot(Vector v) const { return x * v.x + y * v.y; }

float Vector::component(Vector v) const {
	return dot(v) / v.len();
}

Vector Vector::project(Vector v) const {
	return (v * dot(v)) / v.lensqr();
}

float Vector::lensqr() const { return x * x + y * y; }

float Vector::len() const { return sqrt(lensqr()); }

void Vector::normalize() { operator/=(len()); }
Vector Vector::normalized() const { return operator/(len()); }

void Vector::scale(float s) { operator*=(s / len()); }
Vector Vector::scaled(float s) const { return operator*(s / len()); }

Vector Vector::ortho() const { return Vector(-y, x); }

Vector Vector::bisector(Vector v) const {
	Vector bisec = operator*(v.len()) + v * len();
	if (bisec.lensqr() == 0) bisec = ortho();
	return bisec;
}

#define DEG_TO_RAD 0.01745329251f

Vector Vector::rotate(float deg, Vector center) {
	Vector delta = operator-(center);

	deg *= DEG_TO_RAD;

	Vector rotated;
	rotated.x = delta.x * ::cos(deg) - delta.y * ::sin(deg);
	rotated.y = delta.x * ::sin(deg) + delta.y * ::cos(deg);

	return center + rotated;
}

Vector Vector::rotate(Vector direction, Vector center) {
	Vector delta = operator-(center);

	float cos_val = direction.x / direction.len();
	float sin_val = direction.y / direction.len();

	Vector rotated;
	rotated.x = delta.x * cos_val - delta.y * sin_val;
	rotated.y = delta.x * sin_val + delta.y * cos_val;

	return center + rotated;
}

Vector Vector::rotate_inverse(Vector direction, Vector center) {
	Vector delta = operator-(center);

	float cos_val = direction.x / direction.len();
	float sin_val = -direction.y / direction.len();

	Vector rotated;
	rotated.x = delta.x * cos_val - delta.y * sin_val;
	rotated.y = delta.x * sin_val + delta.y * cos_val;

	return center + rotated;
}

Vector Vector::scale(float amount, Vector center) {
	Vector delta = operator-(center);
	delta *= amount;
	return center + delta;
}

Vector Vector::scale(Vector amount, Vector center) {
	Vector delta = operator-(center);
	delta.x *= amount.x;
	delta.y *= amount.y;
	return center + delta;
}

float Vector::angle(Vector other) {
	Vector delta = other.operator-(*this);

	return atan2(delta.y, delta.x);
}

float Vector::cross_len(Vector other) {
	return (x * other.y) - (other.x * y);
}

float Vector::cos(Vector other) {
	return dot(other) / (len() * other.len());
}

float Vector::sin(Vector other) {
	return cross_len(other) / (len() * other.len());
}