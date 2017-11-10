#include "stdafx.h"
#include "Wall.h"


Wall::Wall()
{
}


Wall::~Wall()
{
}

void Wall::InitWall(Vector2 & vPos, Renderer * pRend)
{
	SetPosition(vPos);
	SetRenderer(pRend);
	SetSize(Vector2(52, 52));

}

void Wall::Render()
{
	GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Texture::TextureNumber::Wall);
}
