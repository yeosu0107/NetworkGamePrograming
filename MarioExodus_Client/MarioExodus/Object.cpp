#include "stdafx.h"
#include "Object.h"


Object::Object()
{
}


Object::~Object()
{
}

void Object::SetPosition(Vector2 & pos)
{
	m_vec2Pos = pos;
}

bool Object::Collision(Object & other)
{
	Vector2 vec2marioPos = GetPosition();
	Vector2 vec2otherPos = other.GetPosition();

	int nxdist = vec2otherPos.x - vec2marioPos.x;
	int nydist = vec2otherPos.y - vec2marioPos.y;

	if (nxdist * nxdist + nydist * nydist <= GetSize().x * GetSize().x) return true;

	return false;
}
