#include "stdafx.h"
#include "KeyNDoor.h"


Key::Key()
{
}

Key::Key(Renderer* rend)
{
	SetRenderer(rend);
	SetPosition(Vector2(200, 200));
	m_oDish = Object();
	m_oDish.SetRenderer(rend);
	m_oDish.SetPosition(GetPosition() - Vector2(0, m_oDish.GetPosition().y));
}

Key::~Key()
{
}

void Key::Render()
{
	m_oDish.GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Texture::TextureNumber::Key_Dish);
	
	if (m_bOpen) return;
	GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Texture::TextureNumber::Key);
}

void Key::CollisionMario(Mario & other)
{
}

void Key::CollisionDoor(Door & other)
{
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



///////////////////////////////////////////////////////////

Door::Door()
{
}

Door::Door(Renderer * rend)
{
	SetRenderer(rend);
	SetPosition(Vector2(100, 100));
	SetSize(Vector2(48, 48));
	m_bOpen = true;
}

Door::~Door()
{

}

void Door::Render()
{
	GetRenderer()->DrawSolidRect(GetPosition(), GetSize(),Vector2(m_bOpen,0), Texture::TextureNumber::Door);
}

void Door::CollisionMario(Mario & other)
{
	if (!Collision(other)) return;
	if (m_bOpen)
		other.SetSpriteState(Mario::MarioSprite::Exit);
}
