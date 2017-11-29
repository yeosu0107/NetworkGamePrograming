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

void Mario::InitMario(int nNum, Vector2& vpos)
{
	m_bSelect = false;
	m_iValocity = 5;
	m_iMarioNum = nNum;
	m_fTimePerAction = 8.0f;
	m_bLookDirection = false;
	m_iMarioPlayerNum = -1;
	m_eJumpState = Jump_None;
	m_iMaxJumpDist = 60;
	m_iCurJumpDist = 0;
	m_bCollScreenWall = false;
	m_bCollScreenBott = false;
	m_iUnCollisionCount = 0;
	m_iMinYDistance = 0;
	m_iUnCollsiionXCount = 0;
	m_bExit = false;
	SetObjectType(MARIO);
	
	SetPosition(vpos);
	SetSize(Vector2(52, 52));

	m_oObject.SetPosition(vpos + Vector2(0, 26));
	m_oObject.SetSize(Vector2(20, 16));
}



void Mario::SetSelect(int num)
{
	if (m_iMarioPlayerNum != -1) {
		if(m_iMarioPlayerNum != num)
			return;
	}
		

	m_bSelect = !m_bSelect;
	if (!m_bSelect)
		m_iMarioPlayerNum = -1;
	else
		m_iMarioPlayerNum = num;
}


void Mario::Move(const DWORD byInput)
{	

	if (!m_bSelect)
		return;

	Vector2 vec2pos = GetPosition();
	WORD marioCollside = GetCollSide();

	int xDirection = 0;

	if (byInput & DIR_LEFT)				xDirection += -1;
	if (byInput & DIR_RIGHT)			xDirection += +1;
	if (byInput & KEY_C)				m_eJumpState = m_eJumpState == Jump_None ? Jump_Up : m_eJumpState;	// 마리오가 점프 중이 아닌 상태에서만 반응하게 변경

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

void Mario::Jump()
{
	WORD marioCollside = GetCollSide();

	if (m_eJumpState == Jump_None && marioCollside & CollDown) 
		return;
	else if (m_eJumpState == Jump_None && !(marioCollside & CollDown))
		m_eJumpState = Jump_Down;

	int yDirection = 0;

	Vector2 vec2Direction(0, 1);
	Vector2 vec2pos = GetPosition();

	if (m_eJumpState == Jump_Up) {
		if (marioCollside & CollUp)	
			m_eJumpState = Jump_Down;
		else {
			yDirection += 1;
			m_iCurJumpDist += yDirection * m_iValocity;
		}
	}
	
	else if (m_eJumpState == Jump_Down) {
		if (marioCollside & CollDown)	
			m_eJumpState = Jump_None;
		else						
			yDirection -= 1;
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

void Mario::Update(int iClient, float fTimeElapsed, DWORD dwInputKey)
{
	Jump();

	if (iClient != m_iMarioPlayerNum)
		return;

	Move(dwInputKey);
}

MarioDataFormat Mario::CombinationData()
{
	MarioDataFormat tmp((WORD)m_iMarioNum, m_iMarioPlayerNum, 
		GetPosition().GetX(), GetPosition().GetY(), m_bSelect, m_bLookDirection, m_bExit);
	return tmp;
}