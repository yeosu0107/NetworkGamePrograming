#include "stdafx.h"
#include "KeyNDoor.h"


Key::Key()
{
}

Key::~Key()
{
}


void Key::InitKey(Vector2 & vPos, bool bUsedKey)
{
	SetPosition(vPos);
	SetSize(Vector2(30, 30));
	m_pGrabMario = nullptr;
	m_bUsedKey = bUsedKey;
	m_oDish = Object();
	m_oDish.SetPosition(GetPosition() - Vector2(0, GetSize().y / 2));
	m_oDish.SetSize(Vector2(37, 9));
}

bool Key::CollisionMario(Mario & other)
{
	if (m_bUsedKey)
		return false;

	if (m_pGrabMario != nullptr)
		return false;

	if (Collision(other)) {
		m_pGrabMario = &other;
		return true;
	}
	return false;
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

StageDataFormat Key::CombinationData()
{
	StageDataFormat tmp(-1, GetPosition().GetX(), GetPosition().GetY(), m_bUsedKey);
	return tmp;
}

///////////////////////////////////////////////////////////

Door::Door()
{
}

Door::~Door()
{

}

void Door::InitDoor(Vector2 & vPos, bool bOpen)
{
	SetPosition(vPos);
	SetSize(Vector2(48, 48));
	m_bOpen = bOpen;
}

bool Door::CollisionMario(Mario & other)
{
	if (other.GetExit()) 
		return false;
	if (!Collision(other)) 
		return false;
	if (m_bOpen) {
		other.SetExit(true);
		return true;
	}
	return false;
}
