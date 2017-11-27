#include "stdafx.h"
#include "KeyNDoor.h"


Key::Key()
{
}

Key::~Key()
{
}

void Key::Render()
{
	m_oDish.GetRenderer()->DrawSolidRect(m_oDish.GetPosition(), m_oDish.GetSize(), Vector2(0, 0), Texture::TextureNumber::Key_Dish);
	
	if (m_bUsedKey) return;
	GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Texture::TextureNumber::Key);
}

void Key::InitKey(Vector2 & vPos, bool bUsedKey, Renderer * rend)
{
	SetRenderer(rend);
	SetPosition(vPos);
	SetSize(Vector2(30, 30));
	m_pGrabMario = nullptr;
	m_bUsedKey = bUsedKey;
	m_oDish = Object();
	m_oDish.SetRenderer(rend);
	m_oDish.SetPosition(GetPosition() - Vector2(0, GetSize().y / 2));
	m_oDish.SetSize(Vector2(37, 9));
}

void Key::CollisionMario(Mario & other)
{
	if (m_bUsedKey) return;

	if (Collision(other)) {
		m_pGrabMario = &other;
	}
}


void Key::CollisionDoor(Door & other)
{
	if (m_bUsedKey) return;

	if (Collision(other)) {
		other.SetOpen(true);
		m_bUsedKey = true;
	}
}

void Key::Update(float fTimeElapsed)
{
	if (m_pGrabMario != nullptr)
		SetPosition(m_pGrabMario->GetPosition());
}

void Key::SetMarioPtr(Mario* pMario)
{
	m_pGrabMario = pMario;
}

///////////////////////////////////////////////////////////

Door::Door()
{
}

Door::~Door()
{

}

void Door::Render()
{
	GetRenderer()->DrawSolidRect(GetPosition(), GetSize(),Vector2(m_bOpen,0), Texture::TextureNumber::Door);
}

void Door::InitDoor(Vector2 & vPos, bool bOpen, Renderer* rend)
{
	SetRenderer(rend);
	SetPosition(vPos);
	SetSize(Vector2(48, 48));
	m_bOpen = bOpen;
}

bool Door::CollisionMario(Mario & other)
{
	if (other.IsExit()) return false;
	if (!Collision(other)) return false;
	if (m_bOpen) {
		other.SetSpriteState(Mario::MarioSprite::Exit);
		return true;
	}
	return false;
}
