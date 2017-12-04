#pragma once
#include "stdafx.h"
#include "Renderer.h"
#include "Vector2.h"
#include "Object.h"


class Mario: public Object
{
public:
	
	static enum MarioSprite			{ Sprite_None, Sprite_Run1 , Sprite_Run2 , Sprite_Jump, Exit};	// - Ŭ��
	static enum MarioJumpState		{ Jump_None, Jump_Up, Jump_Down };								// - ����

private:
	int				m_iMarioNum;			// ���������� �ο��� ��( 0 ~ 6 )						- ����, Ŭ��
	int				m_iMaxJumpDist;			// ���� �� �ִ� ����									- ����
	int				m_iCurJumpDist;			// ���� ������ ����									- ����
	int				m_iValocity;			// �̵� �ӵ�											- ����
	int				m_iMarioPlayerNum;		// ����ϴ� �÷��̾� Num								- ����, Ŭ��

	// �ƹ� �浹 ���ϰ� ���߿� �ִ� �������� ����߸��� ���� ����

	float			m_fTimePerAction;		// �ð��� ��������Ʈ ��ȭ �ӵ�							- ����
	float			m_fActionTime;			// �ش� ��������Ʈ�� �ٲ� �� ��� �ð�					- ����

	bool			m_bSelect;				// ������ ���� ����									- ����, Ŭ��
	bool			m_bLookDirection;		// ������ ���� true = ������, false = ����			- ����, Ŭ��
	bool			m_bExit;				// �������� ������ ���� ���

	MarioJumpState	m_eJumpState;			// ������ ���� ����									- ����, Ŭ��
	Object			m_oObject;				// ������ �Ӹ����� �ִ� ���� ��ȣ	������Ʈ				- Ŭ�� 
	MarioSprite		m_eSpriteState;			// ������ ��������Ʈ ����								- ����,Ŭ�� 
	
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

