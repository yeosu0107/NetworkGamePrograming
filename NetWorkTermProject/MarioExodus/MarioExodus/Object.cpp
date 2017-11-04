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
