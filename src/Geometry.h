#ifndef TWIST_GEOMETRY_H_
#define TWIST_GEOMETRY_H_

class Vector {
public:
	float x, y;

	Vector();
	explicit Vector(float);
	Vector(float, float);

	Vector operator+(Vector) const;
	Vector operator-(Vector) const;
	void operator+=(Vector);
	void operator-=(Vector);

	Vector operator*(float) const;
	Vector operator/(float) const;
	void operator*=(float);
	void operator/=(float);

	float dot(Vector) const;

	float component(Vector) const;
	Vector project(Vector) const;

	float len() const;
	float lensqr() const;

	void normalize();
	Vector normalized() const;

	void scale(float);
	Vector scaled(float) const;

	Vector ortho() const;

	Vector bisector(Vector) const;

	Vector rotate(float, Vector);
	Vector rotate(Vector, Vector);
	Vector rotate_inverse(Vector, Vector);

	Vector scale(float, Vector);
	Vector scale(Vector, Vector);

	float angle(Vector);

	float cross_len(Vector);

	float cos(Vector);
	float sin(Vector);
};

#endif