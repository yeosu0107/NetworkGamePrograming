#pragma once
#include "Mario.h"
#include "BackGround.h"
#include "KeyNDoor.h"

class SceneManager
{
private:
	int			m_nStage;
	int			m_nExitMarioCount;

	BackGround	m_bBackGround;
	Door		m_dDoor;
	Key			m_kKey;
	Mario		m_pMario[6];

public:
	SceneManager();
	SceneManager(int nStage, Renderer* pRend);
	~SceneManager();

public:
	void InitSceneManager(Renderer* pRend);
	void Update(float fElapsedTime, DWORD& byInput);
	void Render();
	void CheckObjectCollision(DWORD& byInput);
	void SelectMario(DWORD& bSel);
	void Destroy();

	bool IsClear() { return m_nExitMarioCount == 6; }
	//int RandomCreateObject(const int n, Renderer* rend);

};