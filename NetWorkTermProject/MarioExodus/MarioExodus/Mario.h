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
	int				m_iMaxJumpDist;			// ���� �� �ִ� ����
	int				m_iCurJumpDist;			// ���� ������ ����
	int				m_iMarioNum;			// ���������� �ο��� ��( 0 ~ 6 )
	int				m_iValocity;			// �̵� �ӵ�
	int				m_iPlayerNumber;		// ����ϴ� �÷��̾� Num

	float			m_fTimePerAction;		// �ð��� ��������Ʈ ��ȭ �ӵ�
	float			m_fActionTime;			// �ش� �׼��� ���� �� �ɸ� �ð�

	bool			m_bGrabKey;				// ���踦 ��� �ִ����� ���� ����
	bool			m_bSelect;
	bool			m_bLookDirection;		// ������ ���� true = ������, false = ����
	bool			m_bCollScreenWall;
	bool			m_bCollScreenBott;

	MarioJumpState	m_eJumpState;			// ������ ���� ����
	Object			m_oObject;				// ������ �Ӹ����� �ִ� ���� ��ȣ
	MarioSprite		m_sSpriteState;			// ������ ���¿� ���� ��������Ʈ
	
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

