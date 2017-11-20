#include "stdafx.h"
#include "Wall.h"

void Wall::InitWall(Vector2 & vPos)
{
	SetPosition(vPos);
	SetSize(Vector2(52, 52));

}