#pragma once
#include "stdafx.h"
#include "Renderer.h"
#include "Vector2.h"
#include "Collision.h"
#include "Object.h"


class Mario: public Object
{
	enum MarioJumpState { Jump_None, Jump_Up, Jump_Down };
	enum MarioSprite	{ Sprite_None, Sprite_Run1 , Sprite_Run2 , Sprite_Jump };

private:
	int				m_iMaxJumpDist;			// ���� �� �ִ� ����
	int				m_iMarioNum;			// ���������� �ο��� ��( 0 ~ 6 )
	int				m_iValocity;			// �̵� �ӵ�
	int				m_iPlayerNumber;		// ����ϴ� �÷��̾� Num

	float			m_fTimePerAction;		// �ð��� ��������Ʈ ��ȭ �ӵ�
	float			m_fActionTime;			// �ش� �׼��� ���� �� �ɸ� �ð�

	bool			m_bGrabKey;				// ���踦 ��� �ִ����� ���� ����
	bool			m_bSelect;
	bool			m_bLookDirection;		// ������ ���� true = ������, false = ����

	MarioJumpState	m_eJumpState;			// ������ ���� ����
	Object			m_oObject;				// ������ �Ӹ����� �ִ� ���� ��ȣ
	MarioSprite		m_sSpriteState;			// ������ ���¿� ���� ��������Ʈ
	Collision		m_cCollision;			// AABB
	
public:
	Mario();
	Mario(int num, Vector2& vec2Pos, Renderer* pRenderer);
	~Mario();

	void Update(float fTimeElapsed, DWORD dwInputKey);
	void SpriteUpdate(float fTimeElapsed, DWORD dwInputKey);

	void Move(const float fTimeElapsed, const BYTE byInput);
	void Render();
	void SetSelect() ;
	
	Collision GetCollision() const { return m_cCollision; }

	Mario* CollisionObject(Mario& other);
};

