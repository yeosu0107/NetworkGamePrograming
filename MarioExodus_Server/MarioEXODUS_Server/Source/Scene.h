#pragma once
#include "Mario.h"
#include "Block.h"
#include "KeyNDoor.h"
#include "Block.h"
#include "Wall.h"
#include "stdafx.h"


class Scene
{
private:
	int			m_iExitMarioCount;
	int			m_iwallCount;
	int			m_iBlockCount;

	Door		m_Door;
	Key		m_Key;
	Mario		m_Mario[MaxMario];
	Block*	m_pBlock;
	Wall*		m_pWall;
public:
	Scene() {}
	~Scene();

	void InitScene(int nStage, Vector2* pMarioPos, 
		Vector2& vDoorPos, Vector2& vKeyPos,
		int iWallCount, Vector2* pWallPos,
		int iBlockCount, Vector2* pBlockPos);
	void Update(float fElapsedTime, WORD* byInput1, WORD* byInput2);
	void CheckObjectsCollision();
	void SelectMario(int iClient, WORD& bSel);
	int ApplyObjectsStatus(char* buf);
	void ReadyToNextFrame();

	bool IsClear() const { return m_iExitMarioCount == MaxMario; }
};

