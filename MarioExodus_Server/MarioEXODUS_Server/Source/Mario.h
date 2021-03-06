#pragma once
#include "stdafx.h"
#include "Object.h"


class Mario: public Object
{
public:
	enum MarioJumpState		{ Jump_None, Jump_Up, Jump_Down };								// - 서버

private:
	int				m_iMaxJumpDist;			// 점프 시 최대 높이									- 서버
	int				m_iCurJumpDist;			// 현재 점프한 높이									- 서버
	int				m_iMarioNum;			// 마리오에게 부여된 수( 0 ~ 6 )						- 서버, 클라
	int				m_iValocity;			// 이동 속도											- 서버
	int				m_iMarioPlayerNum;		// 사용하는 플레이어 Num								- 서버, 클라

	// 아무 충돌 안하고 공중에 있는 마리오를 떨어뜨리기 위한 변수
	int				m_iUnCollisionCount;	// 1프레임에 충돌 검사를 진행한 횟수					- 서버
	int				m_iUnCollsiionXCount;	// 마리오가 공중에 있을 때 X좌표 충돌이 안일어나는 오브젝트 갯수 - 서버
	int				m_iMinYDistance;		// 마리오들의 Y좌표를 비교하고 얻은 Y좌표 차이 최소 값	- 서버

	float			m_fTimePerAction;		// 시간당 스프라이트 변화 속도							- 서버
	float			m_fActionTime;			// 해당 스프라이트로 바뀐 뒤 경과 시간					- 서버

	bool			m_bSelect;				// 마리오 선택 여부									- 서버, 클라
	bool			m_bExit;				// 나간 여부
	bool			m_bLookDirection;		// 마리오 방향 true = 오른쪽, false = 왼쪽			- 서버, 클라
	bool			m_bCollScreenWall;		// 화면 좌, 우 충돌 여부, 다중 이동으로 발생하는 충돌체크 오류 현상 해결을 위한 변수	- 서버
	bool			m_bCollScreenBott;		// 화면 상, 하 충돌 여부, 위와 동일					- 서버

	MarioJumpState	m_eJumpState;			// 마리오 점프 상태									- 서버, 클라
	Object			m_oObject;				// 마리오 머리위에 있는 숫자 번호	오브젝트				- 클라 
	
public:
	Mario();
	~Mario();

public:
	void InitMario(int nNum, Vector2& vPos);
	bool IsSelected() const { return m_bSelect; }

	void Update(int iClient, float fTimeElapsed, DWORD dwInputKey);

	void Move(const DWORD byInput);

	void Jump();
	
	
	void SetSelect(int num) ;
	void SetExit(bool bExit) { m_bExit = bExit; }
	bool GetExit() const { return m_bExit; }

	void SetState(Mario::MarioJumpState eState) { m_eJumpState = eState; }

	int getPlayerNum() const { return m_iMarioPlayerNum; }

	MarioDataFormat CombinationData();
};