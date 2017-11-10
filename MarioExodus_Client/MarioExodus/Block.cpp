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

void Block::Update()
{
	Move();
}

void Block::Move()
{
	int iMovePos = 0;

	if (m_iXDir > 0 && !(GetCollSide() & CollRight))	iMovePos += m_iXDir * m_iValocity;
	else if (m_iXDir < 0 && !(GetCollSide() & CollLeft))iMovePos += m_iXDir * m_iValocity;
	
	if (!(GetCollSide() & CollDown)) m_iYDir = -1;

	SetPosition(GetPosition() + Vector2(iMovePos, m_iYDir * m_iValocity));
}
