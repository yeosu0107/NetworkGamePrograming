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
	m_sSpriteState(Sprite_None),
	m_eJumpState(Jump_None),
	m_cCollision(Collision(vec2Pos, 52))
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

	if(m_bSelect)
		GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(m_sSpriteState, m_bLookDirection), Mario_Sprite);
	else
		GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Mario_None);

	GetRenderer()->DrawSolidRect(m_oObject.GetPosition(), m_oObject.GetSize(), Vector2(m_iMarioNum, 0), Marios_Number);
}

void Mario::SetSelect()
{
	m_bSelect = !m_bSelect;
}

void Mario::Move(const float fTimeElapsed, const BYTE byInput)
{
	if (!m_bSelect)
		return;
	
	Vector2 vec2Direction(0, 0);

	if (byInput & DIR_LEFT)				vec2Direction += Vector2(-1, 0);
	if (byInput & DIR_RIGHT)			vec2Direction += Vector2(+1, 0);
	if (byInput & DIR_UP)				m_eJumpState = m_eJumpState == Jump_None ? Jump_Up : m_eJumpState;	// 마리오가 점프 중이 아닌 상태에서만 반응하게 변경
	
	if (vec2Direction.x > 0)			m_bLookDirection = false; 
	else if (vec2Direction.x < 0)		m_bLookDirection = true;

	if (m_eJumpState != Jump_None)		m_sSpriteState = Sprite_Jump;
	else if (vec2Direction.x == 0)		m_sSpriteState = Sprite_None;
	else								m_sSpriteState = (m_sSpriteState == Sprite_None ? Sprite_Run1 : m_sSpriteState);

	Vector2 vec2pos = GetPosition();
	vec2pos += (vec2Direction * m_iValocity);

	SetPosition(vec2pos);
	m_oObject.SetPosition(vec2pos + Vector2(0, 26));
}

void Mario::Update(float fTimeElapsed, DWORD dwInputKey)
{
	Move(fTimeElapsed, dwInputKey);
	SpriteUpdate(fTimeElapsed, dwInputKey);
}

void Mario::SpriteUpdate(float fTimeElapsed, DWORD dwInputKey)
{
	if (dwInputKey == 0 && m_sSpriteState != Sprite_Jump) m_fActionTime = 0.0f;	// 만약 아무키도 안눌려 있고 마리오가 점프 상태가 아닌 경우 프레임 시간을 초기화해준다.
	if (m_sSpriteState == Sprite_None) return;

	if (m_sSpriteState != Sprite_Jump) {
		m_fActionTime += fTimeElapsed;
		m_sSpriteState = (MarioSprite)((int)(m_fActionTime * m_fTimePerAction) % 2 + 1);
	}
}

Mario* CollisionObject(Mario& other);