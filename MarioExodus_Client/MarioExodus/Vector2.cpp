#include "stdafx.h"
#include "Vector2.h"

Vector2::Vector2()
{
	int x = y = 0;
}

Vector2::Vector2(int x, int y) : x(x), y(y)
{
}

Vector2::Vector2(const Vector2& other)
{
	x = other.x;
	y = other.y;
}

Vector2::Vector2(const Vector2* other)
{
	x = other->x; y = other->y;
}

Vector2::~Vector2()
{
}