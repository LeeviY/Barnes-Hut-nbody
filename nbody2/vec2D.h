#pragma once
#include <math.h>

class Vec2D
{
public:
	Vec2D(float _x = 0, float _y = 0) :x(_x), y(_y) {};
	float x;
	float y;

	Vec2D operator + (Vec2D v)
	{
		return Vec2D(x + v.x, y + v.y);
	}

	Vec2D operator / (float f)
	{
		return Vec2D(x / f, y / f);
	}

	void operator += (Vec2D v)
	{
		x += v.x;
		y += v.y;
	}

	void operator *= (float f)
	{
		x *= f;
		y *= f;
	}

	void toUnit()
	{
		float len = sqrt(x * x + y * y);
		x /= len;
		y /= len;
	}
};
/*
Vec2D::Vec2D(float a_x, float a_y)
	:x(a_x)
	, y(a_y)
{}
*/