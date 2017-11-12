#include "stdafx.h"
#include "Mario.h"
#include "Block.h"
#include <algorithm>

Mario::Mario()
{
}

Mario::~Mario()
{
}

void Mario::InitMario(int nNum, Vector2& vpos, Renderer * pRenderer)
{
	m_bSelect = false;
	m_iValocity = 5;
	m_iMarioNum = nNum;
	m_fTimePerAction = 8.0f;
	m_bLookDirection = false;
	m_iMarioPlayerNum = m_iMarioNum % 2;
	m_eSpriteState = Mario::MarioSprite::Sprite_None;
	m_eJumpState = Jump_None;
	m_iMaxJumpDist = 60;
	m_iCurJumpDist = 0;
	SetObjectType(MARIO);
	
	SetPosition(vpos);
	SetSize(Vector2(52, 52));

	m_oObject.SetPosition(vpos + Vector2(0, 26));
	m_oObject.SetSize(Vector2(20, 16));

	SetRenderer(pRenderer);
}

void Mario::Render()
{
	if (m_eSpriteState == Mario::MarioSprite::Exit) return;

	if(m_bSelect)
		GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(m_eSpriteState, m_bLookDirection + m_iMarioPlayerNum * 2), Texture::TextureNumber::Mario_Sprite);
	else
		GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Texture::TextureNumber::Mario_None);

	GetRenderer()->DrawSolidRect(m_oObject.GetPosition(), m_oObject.GetSize(), Vector2(m_iMarioNum, 0), Texture::TextureNumber::Marios_Number);
}

void Mario::SetSelect(int num)
{
	m_bSelect = !m_bSelect;
	m_iMarioPlayerNum = num;
}

void Mario::SetMarioRecvData(RecvMarioDataFormat & rcvData)
{
	m_iMarioPlayerNum = rcvData.iMarioPlayerNum;
	m_bSelect = rcvData.bSelect;
	m_bLookDirection = rcvData.bLookDirection;
	m_eSpriteState = (Mario::MarioSprite)rcvData.eSpriteState;

	Vector2 pos((int)rcvData.wxPos, (int)rcvData.wyPos);
	SetPosition(pos);
}

void Mario::Move(const float fTimeElapsed, const DWORD byInput)
{	

	if (!m_bSelect)
		return;

	Vector2 vec2pos = GetPosition();
	WORD marioCollside = GetCollSide();

	int xDirection = 0;

	if (byInput & DIR_LEFT)				xDirection += -1;
	if (byInput & DIR_RIGHT)			xDirection += +1;
	if (byInput & KEY_C)				m_eJumpState = m_eJumpState == Jump_None ? Jump_Up : m_eJumpState;	// 마리오가 점프 중이 아닌 상태에서만 반응하게 변경

	if (m_eJumpState != Jump_None)		m_eSpriteState = Sprite_Jump;
	else if (xDirection == 0)			m_eSpriteState = Sprite_None;
	else								m_eSpriteState = (m_eSpriteState == Sprite_None ? Sprite_Run1 : m_eSpriteState);

	if (xDirection > 0) {
		m_bLookDirection = false; 
		if (marioCollside & CollRight) xDirection = 0;
	}
	else if (xDirection < 0) { 
		m_bLookDirection = true; 
		if (marioCollside & CollLeft) xDirection = 0;
	}

	vec2pos.x += (xDirection * m_iValocity);


	SetPosition(vec2pos);
}

void Mario::Jump(const float fTimeElapsed)
{
	WORD marioCollside = GetCollSide();

	if (m_eJumpState == Jump_None && marioCollside & CollDown) return;
	else if (m_eJumpState == Jump_None && !(marioCollside & CollDown))
		m_eJumpState = Jump_Down;

	int yDirection = 0;

	Vector2 vec2Direction(0, 1);
	Vector2 vec2pos = GetPosition();

	if (m_eJumpState == Jump_Up) {
		if (marioCollside & CollUp)	m_eJumpState = Jump_Down;
		else {
			yDirection += 1;
			m_iCurJumpDist += yDirection * m_iValocity;
		}
	}
	
	else if (m_eJumpState == Jump_Down) {
		if (marioCollside & CollDown)	m_eJumpState = Jump_None;
		else						yDirection -= 1;
	}

	if (m_iCurJumpDist >= m_iMaxJumpDist)
	{
		m_iCurJumpDist = 0;
		m_eJumpState = Jump_Down;
	}

	if (marioCollside & CollDown && marioCollside & CollUp) {
		m_iCurJumpDist = 0;
		m_eJumpState = Jump_None;
	}

	
	vec2pos.y += yDirection * m_iValocity;
	SetPosition(vec2pos);

}

void Mario::Update(float fTimeElapsed, DWORD dwInputKey)
{
	if (m_eSpriteState == Exit) return;
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
