#pragma once
#include <iostream>

class Vector2D
{
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float, float);

	Vector2D& Add(const Vector2D& vec);
	Vector2D& Sub(const Vector2D& vec);
	Vector2D& Multiply(const Vector2D& vec);
	Vector2D& Divide(const Vector2D& vec);

	friend Vector2D& operator+(Vector2D&, const Vector2D&);
	friend Vector2D& operator-(Vector2D&, const Vector2D&);
	friend Vector2D& operator*(Vector2D&, const Vector2D&);
	friend Vector2D& operator/(Vector2D&, const Vector2D&);

	Vector2D& operator+=(const Vector2D&);
	Vector2D& operator-=(const Vector2D&);
	Vector2D& operator*=(const Vector2D&);
	Vector2D& operator/=(const Vector2D&);

	Vector2D& operator*(const int& i);
	Vector2D& zero();

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vec);
};

