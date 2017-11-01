#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::SceneManager(int nStage, Renderer* pRend) :
	m_iCurrentObjectCount(0),
	m_nStage(nStage)
{
	InitSceneManager(pRend);
}

SceneManager::~SceneManager()
{
	Destroy();
}

void SceneManager::InitSceneManager(Renderer* pRend)
{
	for (int i = 0; i < 6; ++i)
		m_pMario[i] = Mario(i, Vector2((i + 1) * 80, 30), pRend);

	m_bBackGround = BackGround(m_nStage, pRend);
}

void SceneManager::Update(float fElapsedTime, DWORD byInput)
{

	for (Mario& pMa : m_pMario)
		pMa.Update(fElapsedTime, byInput);

}

void SceneManager::Render()
{
	m_bBackGround.Render();

	for (Mario& pMa : m_pMario)
		pMa.Render();
}
void SceneManager::CheckObjectCollision()
{

}

void SceneManager::SelectMario(const DWORD bSel)
{
	m_pMario[bSel].SetSelect();
}

void SceneManager::Destroy()
{
}