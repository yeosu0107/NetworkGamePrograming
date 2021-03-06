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
	m_iMarioNum = nNum;
	m_fTimePerAction = 8.0f;
	m_bLookDirection = false;
	m_iMarioPlayerNum = m_iMarioNum % 2;
	m_eSpriteState = Mario::MarioSprite::Sprite_None;
	m_eJumpState = Jump_None;
	m_bCurJump = false;
	m_bPreJump = false;
	m_bExit = false;

	SetObjectType(MARIO);
	
	SetPosition(vpos);
	SetSize(Vector2(52, 52));

	m_oObject.SetPosition(vpos + Vector2(0, 26));
	m_oObject.SetSize(Vector2(20, 16));

	SetRenderer(pRenderer);
}

void Mario::Render()
{
	if (m_bExit == true) return;

	if(m_bSelect)
		GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(m_eSpriteState, m_bLookDirection + m_iMarioPlayerNum * 2), Texture::TextureNumber::Mario_Sprite);
	else
		GetRenderer()->DrawSolidRect(GetPosition(), GetSize(), Vector2(0, 0), Texture::TextureNumber::Mario_None);

	GetRenderer()->DrawSolidRect(m_oObject.GetPosition(), m_oObject.GetSize(), Vector2(m_iMarioNum, 0), Texture::TextureNumber::Marios_Number);
}



void Mario::SetMarioRecvData(RecvMarioDataFormat & rcvData)
{
	m_iMarioPlayerNum = rcvData.iMarioPlayerNum;
	m_bSelect = rcvData.bSelect;
	m_bLookDirection = rcvData.bLookDirection;
	m_bExit = (Mario::MarioSprite)rcvData.isExit;

	Vector2 pos((int)rcvData.wxPos, (int)rcvData.wyPos);

	SetPrePosition(GetPosition());
	SetPosition(pos);
}

bool Mario::IsStartJump()
{
	if (m_bExit ||!m_bCurJump)	// 점프 중이 아니라면 false를 반환한다. 점프를 시작 했을 때만을 판단하기 위함
		return false;
	
	Vector2 curPos = GetPosition();
	Vector2 prePos = GetPrePosition();

	bool result = (m_bCurJump && !(m_bCurJump == m_bPreJump));

	return result;
}

void Mario::Update(float fTimeElapsed)
{
	if (m_eSpriteState == Exit) return;
	SpriteUpdate(fTimeElapsed);
}

void Mario::SpriteUpdate(float fTimeElapsed)
{
	Vector2 curPos = GetPosition();
	Vector2 prePos = GetPrePosition();

	m_oObject.SetPosition(GetPosition() + Vector2(0, 26));

	if (curPos.x != prePos.x) {
		m_fActionTime += fTimeElapsed;
		m_eSpriteState = (MarioSprite)((int)(m_fActionTime * m_fTimePerAction) % 2 + 1);
	}

	else {
		m_eSpriteState = Sprite_None;
		m_fActionTime = 0.0f;	// 만약 아무키도 안눌려 있고 마리오가 점프 상태가 아닌 경우 프레임 시간을 초기화해준다.
	}

	if (curPos.y != prePos.y) {
		m_eSpriteState = Sprite_Jump;
	}

	m_bPreJump = m_bCurJump;
	m_bCurJump = (curPos.y > prePos.y);	
	/*
		점프를 시작한 조건
			1. y축으로 이동한 거리가 <= 0에서 > 0 으로 바뀐 경우  
	*/
}
