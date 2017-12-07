#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	m_pBlock = NULL;
	m_pWall = NULL;
	m_pSound = NULL;
}


SceneManager::~SceneManager()
{
	Destroy();
}

void SceneManager::InitSceneManager(int nStage, Vector2* pMarioPos,Vector2& vDoorPos, Vector2& vKeyPos, int iWallCount, Vector2* pWallPos, int iBlockCount, 
	Vector2* pBlockPos, Renderer* pRend, SoundManager* pSound)
{
	for (int i = 0; i < MaxMario; i++)
		m_pMario[i].InitMario(i, pMarioPos[i], pRend);

	m_iExitMarioCount = 0;
	m_iBlockCount = iBlockCount;
	m_iWallCount = iWallCount;
	m_pSound = pSound;
	m_bBackGround = BackGround(nStage, pRend);
	m_iPreExitMario = 0;

	bool bReadyStage = nStage == 0 ? true : false;

	m_dDoor.InitDoor(vDoorPos, bReadyStage, pRend);
	m_kKey.InitKey(vKeyPos, bReadyStage, pRend);

	if (m_iBlockCount > 0)	m_pBlock = new Block[m_iBlockCount]();
	for(int i =0;i < m_iBlockCount;i++) m_pBlock[i].InitBlock(pBlockPos[i], pRend);

	if (m_iWallCount > 0)	m_pWall = new Wall[m_iWallCount]();
	for (int i = 0; i < m_iWallCount; i++) m_pWall[i].InitWall(pWallPos[i], pRend);
}

void SceneManager::Update(float fElapsedTime)
{
	for (Mario& pMa : m_pMario) {
		pMa.Update(fElapsedTime);
		if (pMa.IsStartJump()) {
			m_pSound->Play(SoundType::JumpSound);
		}
	}
	if (GetExitMarios() > m_iPreExitMario) {
		m_pSound->Play(SoundType::ExitSound);
		m_iPreExitMario = GetExitMarios();
	}

}

void SceneManager::Render()
{
	m_bBackGround.Render();
	m_dDoor.Render();

	if (m_pBlock != nullptr) 
		for(int i = 0; i < m_iBlockCount; i++)
			m_pBlock[i].Render();

	for (Mario& pMa : m_pMario)
		pMa.Render();

	m_kKey.Render();

	if (m_pWall != nullptr)
		for (int i = 0; i < m_iWallCount; i++)
			m_pWall[i].Render();

}


void SceneManager::Destroy()
{
	if (m_pBlock != nullptr) delete[] m_pBlock;
	if (m_pWall != nullptr) delete[] m_pWall;
}

int SceneManager::GetExitMarios()
{
	int result = 0;
	for (int i = 0; i < MaxMario; ++i)
		if (m_pMario[i].IsExit())
			result++;
	return result;
}

int SceneManager::ApplyObjectsStatus(char* buf)
{
	int   retval = 0;		// 읽은 데이터량 

	RecvStageDataFormat stagedata = *((RecvStageDataFormat*)buf);
	buf += sizeof(RecvStageDataFormat);
	retval += sizeof(RecvStageDataFormat);

	m_kKey.SetPosition(Vector2(stagedata.wKeyXPos, stagedata.wKeyYPos));
	m_kKey.SetKeystatus(stagedata.IsOpen);
	m_dDoor.SetOpen(stagedata.IsOpen);

	for (int i = 0; i < MaxMario; i++) {
		RecvMarioDataFormat mariodata = *((RecvMarioDataFormat*)buf);

		m_pMario[i].SetMarioRecvData(mariodata);
		buf += sizeof(RecvMarioDataFormat);
		retval += sizeof(RecvMarioDataFormat);
	}

	for (int i = 0; i < m_iBlockCount; ++i) {
		RecvStageBlockFormat boxdata = *((RecvStageBlockFormat*)buf);
		m_pBlock[i].SetPosition(Vector2(boxdata.wxPos, boxdata.wyPos));

		buf += sizeof(RecvStageBlockFormat);
		retval += sizeof(RecvStageBlockFormat);
	}
	return retval;
}

/*
75

char Data = 2
struct RecvMarioDataFormat {
char iMarioNum; 2
char iMarioPlayerNum; 2
WORD wxPos; 2
WORD wyPos; 2
bool bSelect;1
bool bLookDirection;1
bool bExit;1
}; 66

struct RecvStageDataFormat {
WORD wKeyXPos;
WORD wKeyYPos;
bool IsOpen;
}; 5 */ 