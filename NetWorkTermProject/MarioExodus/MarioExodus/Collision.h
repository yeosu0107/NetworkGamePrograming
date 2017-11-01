#pragma once
#include "stdafx.h"
#include "Vector2.h"


class Box {
private:
	Vector2 m_vec2BegPoint;
	Vector2 m_vec2EndPoint;

public:
	Box() {};
	Box(Vector2& vec2Beg, Vector2& vec2End) : m_vec2BegPoint(vec2Beg), m_vec2EndPoint(vec2End) {}
	Box(const Vector2& vec2Mid, float size);

	Vector2 getBeg() const { return m_vec2BegPoint; }
	Vector2 getEnd() const { return m_vec2EndPoint; }
};

class Collision {
private:
	Box m_boxAABB;

public:
	Collision() {};
	Collision(Vector2& vec2Beg, Vector2& vec2End) : m_boxAABB(Vector2(vec2Beg), Vector2(vec2End)) {}
	Collision(const Vector2& vec2Mid, float size) : m_boxAABB(vec2Mid, size) {}
public:
	bool IsCollision(const Collision& other);

	void RefreshCollision(Vector2& vMid, float size);

	Box getAABB() const { return m_boxAABB; }
};