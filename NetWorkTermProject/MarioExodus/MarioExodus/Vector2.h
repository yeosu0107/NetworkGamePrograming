#pragma once
class Vector2
{
public:
	int x, y;

public:
	Vector2();
	Vector2(int x, int y);
	Vector2(Vector2&& other) {};
	Vector2(const Vector2& other);
	Vector2(const Vector2* other);
	~Vector2();

public:
	Vector2 GetVector2() { return Vector2(x, y); }
	
	int GetX() const { return x; }
	int GetY() const { return y; }

	void SetX(const int otherx) { x = otherx; }
	void SetY(const int othery) { y = othery; }

public:
	Vector2 operator=(const Vector2& other) {
		x = other.x; y = other.y;
		return Vector2(x, y);
	}

	Vector2 operator+(const Vector2& other) {
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(const Vector2& other) {
		x -= other.x; y -= other.y;
		return Vector2(x, y);;
	}

	Vector2 operator+=(const Vector2& other) {
		x += other.x; y += other.y;
		return Vector2(x, y);
	}

	Vector2 operator-=(const Vector2& other) {
		x -= other.x; y -= other.y;
		return Vector2(x, y);;
	}

	Vector2 operator/(const int value) {
		x /= value; y /= value;
		return Vector2(x, y);
	}

	Vector2 operator*(const int value) {
		x *= value; y *= value;
		return Vector2(x, y);
	}
};

