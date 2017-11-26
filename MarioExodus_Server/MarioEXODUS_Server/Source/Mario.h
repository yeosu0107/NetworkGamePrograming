#pragma once
#include "stdafx.h"
#include "Object.h"


class Mario: public Object
{
public:
	
	enum MarioSprite			{ Sprite_None, Sprite_Run1 , Sprite_Run2 , Sprite_Jump, Exit};	// - Ŭ��
	enum MarioJumpState		{ Jump_None, Jump_Up, Jump_Down };								// - ����

private:
	int				m_iMaxJumpDist;			// ���� �� �ִ� ����									- ����
	int				m_iCurJumpDist;			// ���� ������ ����									- ����
	int				m_iMarioNum;			// ���������� �ο��� ��( 0 ~ 6 )						- ����, Ŭ��
	int				m_iValocity;			// �̵� �ӵ�											- ����
	int				m_iMarioPlayerNum;		// ����ϴ� �÷��̾� Num								- ����, Ŭ��

	// �ƹ� �浹 ���ϰ� ���߿� �ִ� �������� ����߸��� ���� ����
	int				m_iUnCollisionCount;	// 1�����ӿ� �浹 �˻縦 ������ Ƚ��					- ����
	int				m_iUnCollsiionXCount;	// �������� ���߿� ���� �� X��ǥ �浹�� ���Ͼ�� ������Ʈ ���� - ����
	int				m_iMinYDistance;		// ���������� Y��ǥ�� ���ϰ� ���� Y��ǥ ���� �ּ� ��	- ����

	float			m_fTimePerAction;		// �ð��� ��������Ʈ ��ȭ �ӵ�							- ����
	float			m_fActionTime;			// �ش� ��������Ʈ�� �ٲ� �� ��� �ð�					- ����

	bool			m_bSelect;				// ������ ���� ����									- ����, Ŭ��
	bool			m_bExit;				// ���� ����
	bool			m_bLookDirection;		// ������ ���� true = ������, false = ����			- ����, Ŭ��
	bool			m_bCollScreenWall;		// ȭ�� ��, �� �浹 ����, ���� �̵����� �߻��ϴ� �浹üũ ���� ���� �ذ��� ���� ����	- ����
	bool			m_bCollScreenBott;		// ȭ�� ��, �� �浹 ����, ���� ����					- ����

	MarioJumpState	m_eJumpState;			// ������ ���� ����									- ����, Ŭ��
	Object			m_oObject;				// ������ �Ӹ����� �ִ� ���� ��ȣ	������Ʈ				- Ŭ�� 
	MarioSprite		m_eSpriteState;			// ������ ��������Ʈ ����								- ����,Ŭ�� 
	
public:
	Mario();
	~Mario();

public:
	void InitMario(int nNum, Vector2& vPos);
	bool IsSelected() const { return m_bSelect; }

	void Update(int iClient, float fTimeElapsed, DWORD dwInputKey);
	void SpriteUpdate(float fTimeElapsed, DWORD dwInputKey);

	void Move(const DWORD byInput);

	void Jump();
	
	
	void SetSelect(int num) ;
	void SetExit(bool bExit) { m_bExit = bExit; }
	bool GetExit() const { return m_bExit; }

	void SetState(Mario::MarioJumpState eState) { m_eJumpState = eState; }

	//MarioSprite GetSpriteState() const { return m_eSpriteState; }

	void SetMarioRecvData(MarioDataFormat& rcvData);

	int getPlayerNum() const { return m_iMarioPlayerNum; }
};

