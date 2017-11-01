#include "stdafx.h"
#include "Collision.h"


Box::Box(const Vector2& vec2Mid, float size) :
	m_vec2BegPoint(Vector2(vec2Mid.x - size / 2, vec2Mid.y - size / 2)),
	m_vec2EndPoint(Vector2(vec2Mid.x + size / 2, vec2Mid.y + size / 2))
{}

bool Collision::IsCollision(const Collision& other) {
	Vector2 memberBeg = m_boxAABB.getBeg();
	Vector2 memberEnd = m_boxAABB.getEnd();

	Vector2 otherBeg = other.getAABB().getBeg();
	Vector2 otherEnd = other.getAABB().getEnd();

	if (memberBeg.x >= otherEnd.x) return false;
	if (memberBeg.y >= otherEnd.y) return false;
	if (memberEnd.x <= otherBeg.x) return false;
	if (memberEnd.y <= otherBeg.y) return false;

	return true;
}

void Collision::RefreshCollision(Vector2& vMid, float size)
{
	m_boxAABB = Box(vMid, size);
}