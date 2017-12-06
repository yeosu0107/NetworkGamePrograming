#pragma once
#include "Mario.h"
#include "BackGround.h"
#include "KeyNDoor.h"
#include "Block.h"
#include "Wall.h"
#include "SoundManager.h"

class SceneManager
{
private:
	int				m_iExitMarioCount;
	int				m_iWallCount;
	int				m_iBlockCount;
	int				m_iPreExitMario;

	BackGround		m_bBackGround;
	
	Door			m_dDoor;
	Key				m_kKey;
	Mario			m_pMario[MaxMario];
	Block*			m_pBlock;
	Wall*			m_pWall;
	SoundManager*	m_pSound;
public:
	SceneManager();
	~SceneManager();

public:
	void InitSceneManager(int nStage, Vector2* prcvMario, Vector2& vDoorPos, Vector2& vKeyPos, int iWallCount, Vector2* pWallPos, int iBlockCount, 
		Vector2* pBlockPos, Renderer* pRend, SoundManager* pSound);
	void Update(float fElapsedTime, WORD& byInput);
	void Render();
	void Destroy();

	int	 GetExitMarios();
	int  ApplyObjectsStatus(char* buf);

	bool IsClear() { return m_iExitMarioCount == MaxMario; }
};