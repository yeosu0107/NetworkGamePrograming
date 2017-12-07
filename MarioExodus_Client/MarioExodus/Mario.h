#pragma once
#include "stdafx.h"
#include "Renderer.h"
#include "Vector2.h"
#include "Object.h"


class Mario: public Object
{
public:
	
	static enum MarioSprite			{ Sprite_None, Sprite_Run1 , Sprite_Run2 , Sprite_Jump, Exit};	// - Ŭ��
	static enum MarioJumpState		{ Jump_None, Jump_Up, Jump_Down };								// - Ŭ��

private:
	int				m_iMarioNum;			// ���������� �ο��� ��( 0 ~ 6 )						- ����, Ŭ��
	int				m_iMarioPlayerNum;		// ����ϴ� �÷��̾� Num								- ����, Ŭ��

	// �ƹ� �浹 ���ϰ� ���߿� �ִ� �������� ����߸��� ���� ����

	float			m_fTimePerAction;		// �ð��� ��������Ʈ ��ȭ �ӵ�							- Ŭ��
	float			m_fActionTime;			// �ش� ��������Ʈ�� �ٲ� �� ��� �ð�					- Ŭ��

	bool			m_bSelect;				// ������ ���� ����									- ����, Ŭ��
	bool			m_bLookDirection;		// ������ ���� true = ������, false = ����			- ����, Ŭ��
	bool			m_bExit;				// �������� ������ ���� ���
	bool			m_bCurJump;				// ���� ���� ����
	bool			m_bPreJump;				// ���� ������ ���� ����

	MarioJumpState	m_eJumpState;			// ������ ���� ����									- ����, Ŭ��
	Object			m_oObject;				// ������ �Ӹ����� �ִ� ���� ��ȣ	������Ʈ				- Ŭ�� 
	MarioSprite		m_eSpriteState;			// ������ ��������Ʈ ����								- ����,Ŭ�� 
	
public:
	Mario();
	~Mario();

public:
	void InitMario(int nNum, Vector2& vPos, Renderer* pRenderer);
	bool IsSelected() const { return m_bSelect; }
	bool IsJump() const { return m_bCurJump; }
	bool IsStartJump();

	void Update(float fTimeElapsed);
	void SpriteUpdate(float fTimeElapsed);

	void Render();
	
	void SetState(Mario::MarioJumpState eState) { m_eJumpState = eState; }
	void SetSpriteState(Mario::MarioSprite eSprite) { m_eSpriteState = eSprite; }

	void SetMarioRecvData(RecvMarioDataFormat& rcvData);

	MarioSprite GetSpriteState() const { return m_eSpriteState; }

	void SetExit(bool bExit) { m_bExit = bExit; }
	bool IsExit() const { return m_bExit; }
};

