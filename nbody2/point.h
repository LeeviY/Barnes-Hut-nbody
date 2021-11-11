#pragma once
#include "vec2D.h"

struct Point
{
    float x;
    float y;
    float mass = 1;

    Point(float _x, float _y)
    {
        x = _x;
        y = _y;
    }
    Point()
    {
        x = 0;
        y = 0;
    }

    void operator += (Vec2D v)
    {
        x += v.x;
        y += v.y;
    }
    
    Point operator * (float f)
    {
        return Point(f * x, f * y);
    }

    void operator += (Point p)
    {
        x += p.x;
        y += p.y;
    }

    void operator /= (float f)
    {
        x /= f;
        y /= f;
    }
};