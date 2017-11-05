#include "stdafx.h"
#include "Mario.h"


Mario::Mario()
{
}

Mario::Mario(int num, Vector2& vec2Pos, Renderer* pRenderer) :
	m_bSelect(false),
	m_iValocity(5),
	m_iMarioNum(num),
	m_fTimePerAction(8.0f),
	m_bLookDirection(false),
	m_eSpriteState(Sprite_None),
	m_eJumpState(Jump_None),
	m_iMaxJumpDist(70),
	m_iCurJumpDist(0),
	m_bCollScreenWall(false),
	m_bCollScreenBott(false),
	m_iUnCollisionCount(0),
	m_iMinYDistance(0),
	m_iUnCollsiionXCount(0)
{
	SetPosition(vec2Pos);
	SetSize(Vector2(52, 52));

	m_oObject.SetPosition(vec2Pos + Vector2(0, 26));
	m_oObject.SetSize(Vector2(20, 16));

	SetRenderer(pRenderer);
}

Mario::~Mario()
{
}

void Mario::Render()
{
	if (m_eSpriteState == Mario::MarioSprite::Exit) return;

	if(m_bSelect)
		GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(m_eSpriteState, m_bLookDirection), Texture::TextureNumber::Mario_Sprite);
	else
		GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Texture::TextureNumber::Mario_None);

	GetRenderer()->DrawSolidRect(m_oObject.GetPosition(), m_oObject.GetSize(), Vector2(m_iMarioNum, 0), Texture::TextureNumber::Marios_Number);
}

void Mario::SetSelect()
{
	m_bSelect = !m_bSelect;
}

Mario::CollSide Mario::CollisionObject(Mario & other)
{

	bool IsSameMario = (other.m_iMarioNum == m_iMarioNum);

	Vector2 vec2marioPos = GetPosition();
	Vector2 vec2otherPos = other.GetPosition();

	int nxdist = vec2otherPos.x - vec2marioPos.x;
	int nydist = vec2otherPos.y - vec2marioPos.y;

	if (nydist <= 0 && !m_bCollScreenBott && !IsSameMario) {
		if (nxdist * nxdist <= GetSize().x * GetSize().x)
			m_iMinYDistance = min(m_iMinYDistance, -nydist);
	}
	
	else if (nydist > 0 && !other.m_bCollScreenBott && !IsSameMario) {
		if (nxdist * nxdist <= GetSize().x * GetSize().x)
			other.m_iMinYDistance = min(other.m_iMinYDistance, nydist);
	}
	
	if (m_bCollScreenBott) {
		m_iMinYDistance = 0;
	}

	if (IsSameMario) {
		AfterCollision(other, CollNone);
		return CollNone;
	}

	m_iUnCollisionCount++;	
	other.m_iUnCollisionCount++;

	if (nxdist * nxdist + nydist * nydist < GetSize().x * GetSize().x) {
		if (nxdist * nxdist <= nydist * nydist){
			if (nydist > 0) {
				AfterCollision(other, CollUp);
				AfterCollision(other, CollNone);
				return CollUp;
			}
			m_iCurJumpDist = 0;
			AfterCollision(other, CollDown);
			AfterCollision(other, CollNone);
			return CollDown;
		}
		if (nxdist * nxdist > nydist * nydist) {
			if (nxdist > 0) {
				AfterCollision(other, CollRight);
				AfterCollision(other, CollNone);
				return CollRight;
			}
			AfterCollision(other, CollLeft);
			AfterCollision(other, CollNone);
			return CollLeft;
		}	
	}

	// 오브젝트 혼자 공중에 떠있는 것을 내려보내기 위한 작업
	if (nxdist * nxdist > GetSize().x * GetSize().x) {
		if (!m_bCollScreenBott) {
			m_iUnCollsiionXCount++;
		}
	}

	AfterCollision(other, CollNone);
	return CollNone;
}

void Mario::CollisionScreen()
{
	m_bCollScreenWall = false;
	m_bCollScreenBott = false;

	if (GetPosition().x <= GetSize().x / 2) {
		m_bCollScreenWall = true;
		SetPosition(Vector2(GetSize().x / 2, GetPosition().y));
	}
	else if (GetPosition().x >= Screen_Width - GetSize().x / 2) {
		m_bCollScreenWall = true;
		SetPosition(Vector2(Screen_Width - GetSize().x / 2, GetPosition().y));
	}

	if (GetPosition().y <= GetSize().y / 2) {
		m_bCollScreenBott = true;
		SetPosition(Vector2(GetPosition().x, GetSize().y / 2));
		SetState(Mario::MarioJumpState::Jump_None);
	}
	else if (GetPosition().y >= Screen_Height - GetSize().y / 2) {
		m_bCollScreenBott = true;
		SetPosition(Vector2(GetPosition().x, Screen_Height - GetSize().y / 2));
	}

}

void Mario::AfterCollision(Mario & other, CollSide collside)
{
	Vector2 vec2otherPos = other.GetPosition();
	switch (collside)
	{
	case Mario::CollSide::CollDown:
		if(!m_bCollScreenBott)
			SetPosition(Vector2(GetPosition().x, vec2otherPos.y + GetSize().y));
		else {
			other.SetPosition(Vector2(other.GetPosition().x, GetPosition().y - GetSize().y));
			other.m_bCollScreenBott = m_bCollScreenBott;
		}
		SetState(Mario::MarioJumpState::Jump_None);
		break;

	case Mario::CollSide::CollUp:
		if (!m_bCollScreenBott) {
			SetPosition(Vector2(GetPosition().x, vec2otherPos.y - GetSize().y));
			SetState(Mario::MarioJumpState::Jump_Down);
		}
		else {
			other.SetPosition(Vector2(other.GetPosition().x, GetPosition().y + GetSize().y));
			other.m_bCollScreenBott = m_bCollScreenBott;
			SetState(Mario::MarioJumpState::Jump_None);
		}
		
		other.SetState(Mario::MarioJumpState::Jump_None);
		break;

	case Mario::CollSide::CollLeft:

		if (!m_bCollScreenWall)
			SetPosition(Vector2(vec2otherPos.x + GetSize().x, GetPosition().y));
		else {
			other.SetPosition(Vector2(GetPosition().x - GetSize().x, other.GetPosition().y));
			other.m_bCollScreenWall = m_bCollScreenWall;
		}
		break;

	case Mario::CollSide::CollRight:
		if (!m_bCollScreenWall)
			SetPosition(Vector2(vec2otherPos.x - GetSize().x, GetPosition().y));
		else {
			other.SetPosition(Vector2(GetPosition().x + GetSize().x, other.GetPosition().y));
			other.m_bCollScreenWall = m_bCollScreenWall;
		}
		break;

	case Mario::CollSide::CollNone:
		if (m_eJumpState != Jump_None || m_iUnCollisionCount < 5) break;
		if (m_iMinYDistance > GetSize().y)
		{
			std::cout << m_iMinYDistance << " " << m_iMarioNum << std::endl;
			m_eJumpState = Jump_Down;
		}
		else if(m_iUnCollsiionXCount >= 5)  m_eJumpState = Jump_Down;
		break;
	}

}

void Mario::Move(const float fTimeElapsed, const DWORD byInput)
{
	if (!m_bSelect)
		return;
	
	Vector2 vec2pos = GetPosition();

	int xDirection = 0;
	if (byInput & DIR_LEFT)				xDirection += -1;
	if (byInput & DIR_RIGHT)			xDirection += 1;
	if (byInput & KEY_C)				m_eJumpState = m_eJumpState == Jump_None ? Jump_Up : m_eJumpState;	// 마리오가 점프 중이 아닌 상태에서만 반응하게 변경

	if (xDirection > 0)					m_bLookDirection = false;
	else if (xDirection < 0)			m_bLookDirection = true;

	if (m_eJumpState != Jump_None)		m_eSpriteState = Sprite_Jump;
	else if (xDirection == 0)			m_eSpriteState = Sprite_None;
	else								m_eSpriteState = (m_eSpriteState == Sprite_None ? Sprite_Run1 : m_eSpriteState);

	vec2pos.x += (xDirection * m_iValocity);

	SetPosition(vec2pos);
}

void Mario::Jump(const float fTimeElapsed)
{
	if (m_eJumpState == Jump_None) 	return;
	
	Vector2 vec2Direction(0, 1);
	Vector2 vec2pos = GetPosition();

	if (m_eJumpState == Jump_Up) {
		m_iCurJumpDist += m_iValocity;
		vec2pos += (vec2Direction * m_iValocity);
	}

	else if (m_eJumpState == Jump_Down) {
		vec2pos -= (vec2Direction * m_iValocity);
	}

	if (m_iCurJumpDist >= m_iMaxJumpDist)
	{
		m_iCurJumpDist = 0;
		m_eJumpState = Jump_Down;
	}

	SetPosition(vec2pos);

}

void Mario::Update(float fTimeElapsed, DWORD dwInputKey)
{
	if (m_eSpriteState == Exit) return;
	m_iUnCollisionCount = 0;
	m_iMinYDistance = Screen_Height;
	m_iUnCollsiionXCount = 0;

	Move(fTimeElapsed, dwInputKey);
	Jump(fTimeElapsed);
	SpriteUpdate(fTimeElapsed, dwInputKey);
}

void Mario::SpriteUpdate(float fTimeElapsed, DWORD dwInputKey)
{
	m_oObject.SetPosition(GetPosition() + Vector2(0, 26));

	if (dwInputKey == 0 && m_eSpriteState != Sprite_Jump) m_fActionTime = 0.0f;	// 만약 아무키도 안눌려 있고 마리오가 점프 상태가 아닌 경우 프레임 시간을 초기화해준다.
	if (m_eSpriteState == Sprite_None) return;

	if (m_eSpriteState != Sprite_Jump) {
		m_fActionTime += fTimeElapsed;
		m_eSpriteState = (MarioSprite)((int)(m_fActionTime * m_fTimePerAction) % 2 + 1);
	}
}
