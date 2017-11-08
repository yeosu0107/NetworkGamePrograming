#pragma once
#include "Mario.h"
#include "BackGround.h"
#include "KeyNDoor.h"

class SceneManager
{
private:
	int			m_iExitMarioCount;

	BackGround	m_bBackGround;
	Door		m_dDoor;
	Key			m_kKey;
	Mario		m_pMario[MaxMario];

public:
	SceneManager();
	~SceneManager();

public:
	void InitSceneManager(int nStage, Vector2* prcvMario, Vector2& vDoorPos, Vector2& vKeyPos, Renderer* pRend);
	void Update(float fElapsedTime, DWORD& byInput);
	void Render();
	void CheckObjectCollision(DWORD& byInput);
	void SelectMario(DWORD& bSel);
	void Destroy();
	bool IsClear() { return m_iExitMarioCount == MaxMario; }
};