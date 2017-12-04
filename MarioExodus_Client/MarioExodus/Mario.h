#pragma once
#include "stdafx.h"
#include "Renderer.h"
#include "Vector2.h"
#include "Object.h"


class Mario: public Object
{
public:
	
	static enum MarioSprite			{ Sprite_None, Sprite_Run1 , Sprite_Run2 , Sprite_Jump, Exit};	// - 클라
	static enum MarioJumpState		{ Jump_None, Jump_Up, Jump_Down };								// - 서버

private:
	int				m_iMarioNum;			// 마리오에게 부여된 수( 0 ~ 6 )						- 서버, 클라
	int				m_iMaxJumpDist;			// 점프 시 최대 높이									- 서버
	int				m_iCurJumpDist;			// 현재 점프한 높이									- 서버
	int				m_iValocity;			// 이동 속도											- 서버
	int				m_iMarioPlayerNum;		// 사용하는 플레이어 Num								- 서버, 클라

	// 아무 충돌 안하고 공중에 있는 마리오를 떨어뜨리기 위한 변수

	float			m_fTimePerAction;		// 시간당 스프라이트 변화 속도							- 서버
	float			m_fActionTime;			// 해당 스프라이트로 바뀐 뒤 경과 시간					- 서버

	bool			m_bSelect;				// 마리오 선택 여부									- 서버, 클라
	bool			m_bLookDirection;		// 마리오 방향 true = 오른쪽, false = 왼쪽			- 서버, 클라
	bool			m_bExit;				// 마리오가 문밖을 나간 경우

	MarioJumpState	m_eJumpState;			// 마리오 점프 상태									- 서버, 클라
	Object			m_oObject;				// 마리오 머리위에 있는 숫자 번호	오브젝트				- 클라 
	MarioSprite		m_eSpriteState;			// 마리오 스프라이트 상태								- 서버,클라 
	
public:
	Mario();
	~Mario();

public:
	void InitMario(int nNum, Vector2& vPos, Renderer* pRenderer);
	bool IsSelected() const { return m_bSelect; }

	void Update(float fTimeElapsed, DWORD dwInputKey);
	void SpriteUpdate(float fTimeElapsed, DWORD dwInputKey);

	void Render();
	
	void SetState(Mario::MarioJumpState eState) { m_eJumpState = eState; }
	void SetSpriteState(Mario::MarioSprite eSprite) { m_eSpriteState = eSprite; }

	void SetMarioRecvData(RecvMarioDataFormat& rcvData);

	MarioSprite GetSpriteState() const { return m_eSpriteState; }

	void SetExit(bool bExit) { m_bExit = bExit; }
	bool IsExit() const { return m_bExit; }
};

