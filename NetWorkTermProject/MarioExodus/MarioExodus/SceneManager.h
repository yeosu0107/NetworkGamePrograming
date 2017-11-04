#pragma once
#include "Mario.h"
#include "BackGround.h"
#include "KeyNDoor.h"

class SceneManager
{
private:
	int			m_iCurrentObjectCount;
	int			m_nStage;

	BackGround	m_bBackGround;
	Door		m_dDoor;
	Mario		m_pMario[6];

public:
	SceneManager();
	SceneManager(int nStage, Renderer* pRend);
	~SceneManager();

public:
	void InitSceneManager(Renderer* pRend);
	void Update(float fElapsedTime, DWORD byInput);
	void Render();
	void CheckObjectCollision();
	void SelectMario(const DWORD bSel);
	void Destroy();
	//int RandomCreateObject(const int n, Renderer* rend);

};