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
	m_dDoor = Door(pRend);
}

void SceneManager::Update(float fElapsedTime, DWORD byInput)
{
	for (Mario& pMa : m_pMario)
		pMa.Update(fElapsedTime, byInput);
}

void SceneManager::Render()
{
	m_bBackGround.Render();
	m_dDoor.Render();

	for (Mario& pMa : m_pMario)
		pMa.Render();
}
void SceneManager::CheckObjectCollision()
{
	std::vector<Mario*> vecSelecMario;
	std::vector<Mario*> vecUnSelecMario;

	for (int i = 0; i < 6; i++) {
		if (m_pMario[i].IsSelected())
			vecSelecMario.emplace_back(&m_pMario[i]);
		else
			vecUnSelecMario.emplace_back(&m_pMario[i]);
	}

	/* 
	 *	선택받은 마리오랑 안받은 마리오 우선 충돌 체크
	 *	기존 : 부여된 숫자에 따라 충돌 체크 하기에 선택 안된 마리오가 충돌 검사를 받고 이동 되는 경우가 많았음 
	 */
	for (UINT i = 0; i < vecSelecMario.size(); ++i)
	{
		vecSelecMario[i]->CollisionScreen();

		for (UINT j = 0; j < vecUnSelecMario.size(); ++j)
		{
			Mario::CollSide side = vecSelecMario[i]->CollisionObject(*vecUnSelecMario[j]);
		}
	}

	/*
	 *	선택받은 마리오끼리 충돌 체크
	 */
	for (UINT i = 0; i < vecSelecMario.size(); ++i)
	{
		for (UINT j = i + 1; j < vecSelecMario.size(); ++j)
		{
			Mario::CollSide side = vecSelecMario[i]->CollisionObject(*vecSelecMario[j]);
		}
	}
}

void SceneManager::SelectMario(const DWORD bSel)
{
	m_pMario[bSel].SetSelect();
}

void SceneManager::Destroy()
{
}