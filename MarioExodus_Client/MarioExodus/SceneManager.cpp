#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	Destroy();
}

void SceneManager::InitSceneManager(int nStage, Vector2* pMarioPos,Vector2& vDoorPos, Vector2& vKeyPos, Renderer* pRend)
{
	for (int i = 0; i < MaxMario; i++)
		m_pMario[i].InitMario(i, pMarioPos[i], pRend);

	m_iExitMarioCount = 0;
	m_bBackGround = BackGround(nStage, pRend);

	bool bReadyStage = nStage == 0 ? true : false;

	m_dDoor.InitDoor(vDoorPos, bReadyStage, pRend);
	m_kKey.InitKey(vKeyPos, bReadyStage, pRend);
}

void SceneManager::Update(float fElapsedTime, DWORD& byInput)
{
	SelectMario(byInput);
	
	for (Mario& pMa : m_pMario)
		pMa.Update(fElapsedTime, byInput);

	m_kKey.Update(fElapsedTime);
}

void SceneManager::Render()
{
	m_bBackGround.Render();
	m_dDoor.Render();

	for (Mario& pMa : m_pMario)
		pMa.Render();

	m_kKey.Render();
}
void SceneManager::CheckObjectCollision(DWORD& byInput)
{
	std::vector<Mario*> vecSelecMario;
	std::vector<Mario*> vecUnSelecMario;

	m_kKey.CollisionDoor(m_dDoor);

	UINT i, j;

	for (i = 0; i < MaxMario; i++) {
		
		if (m_dDoor.CollisionMario(m_pMario[i])) {
			m_iExitMarioCount++;
			m_pMario[i].SetExit(true);
		}
		if (m_pMario[i].IsSelected())
			vecSelecMario.emplace_back(&m_pMario[i]);
		else
			vecUnSelecMario.emplace_back(&m_pMario[i]);
	}

	for(i = 0; i < vecUnSelecMario.size(); ++i)
		(*vecUnSelecMario[i]).CollisionScreen();

	/* 
	 *	선택받은 마리오랑 안받은 마리오 우선 충돌 체크
	 *	기존 : 부여된 숫자에 따라 충돌 체크 하기에 선택 안된 마리오가 충돌 검사를 받고 이동 되는 경우가 많았음 
	 */
	for (i = 0; i < vecSelecMario.size(); ++i)
	{
		if (byInput & KEY_X) m_kKey.CollisionMario(*vecSelecMario[i]);
		else m_kKey.SetMarioPtr(nullptr);

		vecSelecMario[i]->CollisionScreen();

		for (j = 0; j < vecUnSelecMario.size(); ++j)
		{
			if (vecSelecMario[i]->GetSpriteState() == Mario::MarioSprite::Exit 
				|| vecUnSelecMario[j]->GetSpriteState() == Mario::MarioSprite::Exit) 
				continue;

			Mario::CollSide side = vecSelecMario[i]->CollisionObject(*vecUnSelecMario[j]);
		}
	}

	/*
	 *	선택받은 마리오끼리 충돌 체크
	 */
	for (i = 0; i < vecUnSelecMario.size(); ++i)
	{
		for (j = i + 1; j < vecUnSelecMario.size(); ++j)
		{
			if (vecUnSelecMario[i]->GetSpriteState() == Mario::MarioSprite::Exit
				|| vecUnSelecMario[j]->GetSpriteState() == Mario::MarioSprite::Exit)
				continue;

			Mario::CollSide side = vecUnSelecMario[i]->CollisionObject(*vecUnSelecMario[j]);
		}
	}

	if (i > 0)
		vecUnSelecMario[i - 1]->CollisionObject(*vecUnSelecMario[i - 1]);

	/*
	 *	선택 못 받은 마리오끼리 충돌 체크
	 */
	for (i = 0; i < vecSelecMario.size(); ++i)
	{
		for (j = i+1; j < vecSelecMario.size(); ++j)
		{
			if (vecSelecMario[i]->GetSpriteState() == Mario::MarioSprite::Exit
				|| vecSelecMario[j]->GetSpriteState() == Mario::MarioSprite::Exit)
				continue;

			Mario::CollSide side = vecSelecMario[i]->CollisionObject(*vecSelecMario[j]);
		}
	}

	if (i > 0) 
		vecSelecMario[i - 1]->CollisionObject(*vecSelecMario[i - 1]);
}

void SceneManager::SelectMario(DWORD& bSel)
{
	if (bSel % 0x40 == 0) return;
	if (bSel & KEY_1) { m_pMario[0].SetSelect(); };
	if (bSel & KEY_2) { m_pMario[1].SetSelect(); };
	if (bSel & KEY_3) { m_pMario[2].SetSelect(); };
	if (bSel & KEY_4) { m_pMario[3].SetSelect(); };
	if (bSel & KEY_5) { m_pMario[4].SetSelect(); };
	if (bSel & KEY_6) { m_pMario[5].SetSelect(); };

	bSel &= !(0x40 - 1);	// 0x63까지의 비트를 반전 시키고 입력 키값과 And 연산 = 하위 6비트 초기화
	
}

void SceneManager::Destroy()
{
}