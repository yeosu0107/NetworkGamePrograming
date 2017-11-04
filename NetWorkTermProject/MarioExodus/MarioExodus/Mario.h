#pragma once
#include "stdafx.h"
#include "Renderer.h"
#include "Vector2.h"
#include "Object.h"


class Mario: public Object
{
public:
	
	static enum MarioSprite			{ Sprite_None, Sprite_Run1 , Sprite_Run2 , Sprite_Jump, Exit};
	static enum MarioJumpState		{ Jump_None, Jump_Up, Jump_Down };
	static enum CollSide			{ CollNone, CollLeft, CollRight, CollDown, CollUp };

private:
	int				m_iMaxJumpDist;			// 점프 시 최대 높이
	int				m_iCurJumpDist;			// 현재 점프한 높이
	int				m_iMarioNum;			// 마리오에게 부여된 수( 0 ~ 6 )
	int				m_iValocity;			// 이동 속도
	int				m_iPlayerNumber;		// 사용하는 플레이어 Num

	float			m_fTimePerAction;		// 시간당 스프라이트 변화 속도
	float			m_fActionTime;			// 해당 액션을 취한 뒤 걸린 시간

	bool			m_bGrabKey;				// 열쇠를 잡고 있는지에 대한 여부
	bool			m_bSelect;
	bool			m_bLookDirection;		// 마리오 방향 true = 오른쪽, false = 왼쪽
	bool			m_bCollScreenWall;
	bool			m_bCollScreenBott;

	MarioJumpState	m_eJumpState;			// 마리오 점프 상태
	Object			m_oObject;				// 마리오 머리위에 있는 숫자 번호
	MarioSprite		m_sSpriteState;			// 마리오 상태에 따른 스프라이트
	
public:
	Mario();
	Mario(int num, Vector2& vec2Pos, Renderer* pRenderer);
	~Mario();

	bool IsSelected() const { return m_bSelect; }

	void Update(float fTimeElapsed, DWORD dwInputKey);
	void SpriteUpdate(float fTimeElapsed, DWORD dwInputKey);

	void Move(const float fTimeElapsed, const DWORD byInput);

	void Jump(const float fTimeElapsed);
	void Render();
	
	void SetSelect() ;

	void SetState(Mario::MarioJumpState eState) { m_eJumpState = eState; }
	void SetSpriteState(Mario::MarioSprite eSprite) { m_sSpriteState = eSprite; }

	MarioSprite GetSpriteState() const { return m_sSpriteState; }

	CollSide CollisionObject(Mario& other);
	void CollisionScreen();
	void AfterCollision(Object& other, CollSide collside);
};

