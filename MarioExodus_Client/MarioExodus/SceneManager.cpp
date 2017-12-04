#include "stdafx.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
	Destroy();
}

void SceneManager::InitSceneManager(int nStage, Vector2* pMarioPos,Vector2& vDoorPos, Vector2& vKeyPos, int iWallCount, Vector2* pWallPos, int iBlockCount, Vector2* pBlockPos, Renderer* pRend)
{
	for (int i = 0; i < MaxMario; i++)
		m_pMario[i].InitMario(i, pMarioPos[i], pRend);

	m_iExitMarioCount = 0;
	m_iBlockCount = iBlockCount;
	m_iWallCount = iWallCount;
	m_bBackGround = BackGround(nStage, pRend);

	bool bReadyStage = nStage == 0 ? true : false;

	m_dDoor.InitDoor(vDoorPos, bReadyStage, pRend);
	m_kKey.InitKey(vKeyPos, bReadyStage, pRend);

	if (m_iBlockCount > 0)	m_pBlock = new Block[m_iBlockCount]();
	for(int i =0;i < m_iBlockCount;i++) m_pBlock[i].InitBlock(pBlockPos[i], pRend);

	if (m_iWallCount > 0)	m_pWall = new Wall[m_iWallCount]();
	for (int i = 0; i < m_iWallCount; i++) m_pWall[i].InitWall(pWallPos[i], pRend);
}

void SceneManager::Update(float fElapsedTime, WORD& byInput)
{
	
	for (Mario& pMa : m_pMario)
		pMa.Update(fElapsedTime, byInput);

	m_kKey.Update(fElapsedTime);
	
	if (m_pBlock != nullptr)
		for (int i = 0; i < m_iBlockCount; i++)
			m_pBlock[i].Update();
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
void SceneManager::CheckObjectCollision(WORD& byInput)
{
	
	for (int i = 0; i < MaxMario; i++) {
		if (m_pMario[i].IsExit()) continue;	// 마리오가 나간경우 충돌 체크 X

		//if (byInput & KEY_X) m_kKey.CollisionMario(m_pMario[i]);				// X키가 눌린 경우 열쇠와 현재 마리오와 충돌 검사
		//else m_kKey.SetMarioPtr(nullptr);										// 아닌 경우 열쇠가 마리오를 쫓지 않게 변경 

		if (m_dDoor.CollisionMario(m_pMario[i])) m_iExitMarioCount++;			// 마리오가 열린 문과 충돌한 경우

		m_pMario[i].CollisionScreen();

		for (int j = 0; j < m_iBlockCount; ++j) {
			m_pBlock[j].Collision(m_pMario[i]);
			m_pMario[i].Collision(m_pBlock[j]);
		}

		for( int j = 0; j < m_iWallCount; ++j)	
			m_pMario[i].Collision(m_pWall[j]);

		for (int j = 0; j < MaxMario; j++) 
			if (!m_pMario[j].IsExit() && i != j) m_pMario[i].Collision(m_pMario[j]);
		

		m_pMario[i].AfterCollision();	// 이동 후 Box오브젝트을 밀거나 Y축에 대해서만 후처리
	}

	//
	for (int i = 0; i < m_iBlockCount; ++i) {
		for(int j = 0; j < m_iWallCount; ++j)
			m_pBlock[i].Collision(m_pWall[j]);

		//for (int j = 0; j < m_iBlockCount; ++j) 
		//	if (i != j) m_pBlock[i].Collision(m_pBlock[j]);
		
		m_pBlock[i].AfterCollision();
		m_pBlock[i].CollisionScreen();

	}
	m_kKey.CollisionDoor(m_dDoor);
}


void SceneManager::Destroy()
{
	if (m_pBlock != nullptr) delete[] m_pBlock;
	if (m_pWall != nullptr) delete[] m_pWall;
}

void SceneManager::ReadyToNextFrame()
{
	for (int i = 0; i < MaxMario; i++) {
		m_pMario[i].GetCollObjects().clear();	// 충돌한 객체 초기화
		m_pMario[i].SetCollside(0);		// 충돌한 방향 초기화
	}

	for (int i = 0; i < m_iBlockCount; ++i) {
		m_pBlock[i].SetXDir(0);
		m_pBlock[i].SetCollside(0);
		m_pBlock[i].GetCollObjects().clear();
		m_pBlock[i].SetYDir(0);
	}
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