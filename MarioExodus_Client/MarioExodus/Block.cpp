#include "stdafx.h"
#include "Block.h"


Block::Block()
{
}


Block::~Block()
{
}

void Block::InitBlock(Vector2 & vPos, Renderer * pRend)
{
	m_iValocity = 5;
	m_iXDir = 0;
	m_iYDir = 0;
	SetRenderer(pRend);
	SetPosition(vPos);
	SetSize(Vector2(52, 52));
	SetObjectType(BLOCK);
}

void Block::Render()
{
	GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Texture::TextureNumber::Block);
}

