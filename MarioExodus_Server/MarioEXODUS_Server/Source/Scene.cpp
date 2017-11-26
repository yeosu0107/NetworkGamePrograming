#include "Scene.h"

Scene::~Scene()
{
	if(m_pBlock) delete m_pBlock;
	if(m_pWall) delete m_pWall;
}

void Scene::InitScene(int nStage, Vector2* pMarioPos,
	Vector2& vDoorPos, Vector2& vKeyPos,
	int iWallCount, Vector2* pWallPos,
	int iBlockCount, Vector2* pBlockPos)
{
	for (int i = 0; i < MaxMario; ++i) {
		m_Mario[i].InitMario(i, pMarioPos[i]);
	}
	m_iExitMarioCount = 0;
	m_iBlockCount = iBlockCount;
	m_iwallCount = iWallCount;

	bool bReadyStage = nStage == 0 ? true : false;

	m_Door.InitDoor(vDoorPos, bReadyStage);
	m_Key.InitKey(vKeyPos, bReadyStage);

	if (m_iBlockCount > 0) {
		m_pBlock = new Block[m_iBlockCount];
		for (int i = 0; i < m_iBlockCount; ++i) {
			m_pBlock[i].InitBlock(pBlockPos[i]);
		}
	}

	if (m_iwallCount > 0) {
		m_pWall = new Wall[m_iwallCount];
		for (int i = 0; i < m_iwallCount; ++i) {
			m_pWall[i].InitWall(pWallPos[i]);
		}
	}
}

void Scene::Update(float fElapsedTime, WORD* byInput1, WORD* byInput2)
{
	WORD* byInput[2];
	byInput[0] = byInput1;
	byInput[1] = byInput2;
	//마리오
	for (int i = 0; i < 2; ++i) {
		if (byInput[i] == nullptr)
			continue;
		
		SelectMario(i, byInput[i]);
		for (Mario& pMa : m_Mario) {
			if (pMa.getPlayerNum() != i)
				continue;

			pMa.Update(i, fElapsedTime, *byInput[i]);
		}
	}
	//키
	m_Key.Update(fElapsedTime);
	//블록
	if (m_pBlock != nullptr)
		for (int i = 0; i < m_iBlockCount; i++)
			m_pBlock[i].Update();
}

void Scene::CheckObjectsCollision(WORD* byInput1, WORD* byInput2)
{
	WORD* byInput[2];
	byInput[0] = byInput1;
	byInput[1] = byInput2;

	for (int i = 0; i < MaxMario; i++) {
		if (m_Mario[i].GetExit()) 
			continue;	// 마리오가 나간경우 충돌 체크 X

		m_Key.SetMarioPtr(nullptr);
		for (int i = 0; i < 2; ++i) {
			if (byInput[i] != nullptr) {
				if (*byInput[i] & KEY_X)
					m_Key.CollisionMario(m_Mario[i]);
				else
					m_Key.SetMarioPtr(nullptr);
			}
		}

		if (m_Door.CollisionMario(m_Mario[i]))
			m_iExitMarioCount++;			// 마리오가 열린 문과 충돌한 경우

		m_Mario[i].CollisionScreen();

		for (int j = 0; j < m_iBlockCount; ++j) {
			m_pBlock[j].Collision(m_Mario[i]);
			m_Mario[i].Collision(m_pBlock[j]);
		}

		for (int j = 0; j < m_iwallCount; ++j)
			m_Mario[i].Collision(m_pWall[j]);

		for (int j = 0; j < MaxMario; j++)
			if (!m_Mario[j].GetExit() && i != j) 
				m_Mario[i].Collision(m_Mario[j]);


		m_Mario[i].AfterCollision();	// 이동 후 Box오브젝트을 밀거나 Y축에 대해서만 후처리
	}

	//
	for (int i = 0; i < m_iBlockCount; ++i) {
		for (int j = 0; j < m_iwallCount; ++j)
			m_pBlock[i].Collision(m_pWall[j]);

		for (int j = 0; j < m_iBlockCount; ++j)
			if (i != j) m_pBlock[i].Collision(m_pBlock[j]);

		m_pBlock[i].AfterCollision();
		m_pBlock[i].CollisionScreen();

	}
	m_Key.CollisionDoor(m_Door);
}

void Scene::SelectMario(int iClient, WORD* bSel)
{
	//iClient <Player1=0, Player2=1>
	if (iClient > 1 || iClient < 0)
		return;
	if (*bSel % 0x40 == 0) return;
	if (*bSel & KEY_1) 
		m_Mario[0].SetSelect(iClient); 
	if (*bSel & KEY_2) 
		m_Mario[1].SetSelect(iClient);
	if (*bSel & KEY_3) 
		m_Mario[2].SetSelect(iClient);
	if (*bSel & KEY_4) 
		m_Mario[3].SetSelect(iClient); 
	if (*bSel & KEY_5) 
		m_Mario[4].SetSelect(iClient); 
	if (*bSel & KEY_6) 
		m_Mario[5].SetSelect(iClient); 

	// 0x63까지의 비트를 반전 시키고 입력 키값과 And 연산 = 하위 6비트 초기화
	*bSel &= !(0x40 - 1);
}


int Scene::ApplyObjectsStatus(char* buf)
{
	char* tmpbuf = buf;
	int retval = 0;

	for (int i = 0; i < MaxMario; i++) {
		MarioDataFormat mariodata = *((MarioDataFormat*)&tmpbuf);

		m_Mario[i].SetMarioRecvData(mariodata);
		tmpbuf += sizeof(MarioDataFormat);
		retval += sizeof(MarioDataFormat);
	}

	StageDataFormat stagedata = *((StageDataFormat*)&tmpbuf);

	m_Key.SetPosition(Vector2(stagedata.wKeyXPos, stagedata.wKeyYPos));
	m_Key.SetKeystatus(stagedata.IsOpen);
	m_Door.SetOpen(stagedata.IsOpen);
	tmpbuf += sizeof(MarioDataFormat);
	retval += sizeof(MarioDataFormat);

	return retval;
}

void Scene::ReadyToNextFrame()
{
	for (int i = 0; i < MaxMario; i++) {
		m_Mario[i].GetCollObjects().clear();	// 충돌한 객체 초기화
		m_Mario[i].SetCollside(0);				// 충돌한 방향 초기화
	}

	for (int i = 0; i < m_iBlockCount; ++i) {
		m_pBlock[i].SetXDir(0);					// 박스에게 X축으로 가해진 힘의 방향 초기화
		m_pBlock[i].SetCollside(0);				// 박스가 충돌한 방향 초기화
		m_pBlock[i].GetCollObjects().clear();	// 박스와 충돌한 객체 초기화
		m_pBlock[i].SetYDir(0);					// 박스에게 Y축으로 가해진 힘의 방향 초기화
	}
}