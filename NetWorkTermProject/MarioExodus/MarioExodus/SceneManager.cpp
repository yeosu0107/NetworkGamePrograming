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
	 *	���ù��� �������� �ȹ��� ������ �켱 �浹 üũ
	 *	���� : �ο��� ���ڿ� ���� �浹 üũ �ϱ⿡ ���� �ȵ� �������� �浹 �˻縦 �ް� �̵� �Ǵ� ��찡 ������ 
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
	 *	���ù��� ���������� �浹 üũ
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