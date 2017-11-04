#include "stdafx.h"
#include "KeyNDoor.h"


Key::Key()
{
}

Key::Key(int iStage, Renderer* rend)
{
	SetRenderer(rend);
	SetPosition(Vector2(200, 200));
	SetSize(Vector2(30, 30));
	m_pGrabMario = nullptr;
	m_bOpen = false;
	m_oDish = Object();
	m_oDish.SetRenderer(rend);
	m_oDish.SetPosition(GetPosition() - Vector2(0, GetSize().y / 2));
	m_oDish.SetSize(Vector2(37, 9));
}

Key::~Key()
{
}

void Key::Render()
{
	m_oDish.GetRenderer()->DrawSolidRect(m_oDish.GetPosition(), m_oDish.GetSize(), Vector2(0, 0), Texture::TextureNumber::Key_Dish);
	
	if (m_bOpen) return;
	GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Texture::TextureNumber::Key);
}

void Key::CollisionMario(Mario & other)
{
	if (m_bOpen) return; 

	if (Collision(other)) {
		m_pGrabMario = &other;
	}
}


void Key::CollisionDoor(Door & other)
{
	if (m_bOpen) return;

	if (Collision(other)) {
		other.SetOpen(true);
		m_bOpen = true;
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

Door::Door(int iStage, Renderer * rend)
{
	SetRenderer(rend);
	SetPosition(Vector2(100, 100));
	SetSize(Vector2(48, 48));
	m_bOpen = iStage == 0 ? false : false;
}

Door::~Door()
{

}

void Door::Render()
{
	GetRenderer()->DrawSolidRect(GetPosition(), GetSize(),Vector2(m_bOpen,0), Texture::TextureNumber::Door);
}

bool Door::CollisionMario(Mario & other)
{
	if (other.GetSpriteState() == Mario::MarioSprite::Exit) return false;
	if (!Collision(other)) return false;
	if (m_bOpen) {
		other.SetSpriteState(Mario::MarioSprite::Exit);
		return true;
	}
	return false;
}
