#include "stdafx.h"
#include "Object.h"
#include "Block.h"


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

void Object::SetPrePosition(Vector2 & pos)
{
	m_vec2PrePos = pos;
}
