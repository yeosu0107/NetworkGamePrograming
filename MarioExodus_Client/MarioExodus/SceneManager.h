#pragma once
#include "Mario.h"
#include "BackGround.h"
#include "KeyNDoor.h"
#include "Block.h"
#include "Wall.h"

class SceneManager
{
private:
	int			m_iExitMarioCount;
	int			m_iWallCount;
	int			m_iBlockCount;

	BackGround	m_bBackGround;
	Door		m_dDoor;
	Key			m_kKey;
	Mario		m_pMario[MaxMario];
	Block*		m_pBlock;
	Wall*		m_pWall;

public:
	SceneManager();
	~SceneManager();

public:
	void InitSceneManager(int nStage, Vector2* prcvMario, Vector2& vDoorPos, Vector2& vKeyPos, int iWallCount, Vector2* pWallPos, int iBlockCount, Vector2* pBlockPos, Renderer* pRend);
	void Update(float fElapsedTime, WORD& byInput);
	void Render();
	void CheckObjectCollision(WORD& byInput);
	void Destroy();
	void ReadyToNextFrame();

	int  ApplyObjectsStatus(char* buf);

	bool IsClear() { return m_iExitMarioCount == MaxMario; }
};