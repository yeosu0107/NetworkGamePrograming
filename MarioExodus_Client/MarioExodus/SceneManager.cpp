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
	SelectMario(byInput);
	
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
		if (m_pMario[i].GetSpriteState() == Mario::MarioSprite::Exit) continue;	// �������� ������� �浹 üũ X

		if (byInput & KEY_X) m_kKey.CollisionMario(m_pMario[i]);				// XŰ�� ���� ��� ����� ���� �������� �浹 �˻�
		else m_kKey.SetMarioPtr(nullptr);										// �ƴ� ��� ���谡 �������� ���� �ʰ� ���� 

		if (m_dDoor.CollisionMario(m_pMario[i])) m_iExitMarioCount++;			// �������� ���� ���� �浹�� ���

		m_pMario[i].CollisionScreen();

		for (int j = 0; j < m_iBlockCount; ++j) {
			m_pBlock[j].Collision(m_pMario[i]);
			m_pMario[i].Collision(m_pBlock[j]);
		}

		for( int j = 0; j < m_iWallCount; ++j)	
			m_pMario[i].Collision(m_pWall[j]);

		for (int j = 0; j < MaxMario; j++) 
			if (m_pMario[j].GetSpriteState() != Mario::MarioSprite::Exit && i != j) m_pMario[i].Collision(m_pMario[j]);
		

		m_pMario[i].AfterCollision();	// �̵� �� Box������Ʈ�� �аų� Y�࿡ ���ؼ��� ��ó��
	}

	//
	for (int i = 0; i < m_iBlockCount; ++i) {
		for(int j = 0; j < m_iWallCount; ++j)
			m_pBlock[i].Collision(m_pWall[j]);

		for (int j = 0; j < m_iBlockCount; ++j) 
			if (i != j) m_pBlock[i].Collision(m_pBlock[j]);
		
		m_pBlock[i].AfterCollision();
		m_pBlock[i].CollisionScreen();

	}
	m_kKey.CollisionDoor(m_dDoor);
}

void SceneManager::SelectMario(WORD& bSel)
{
	if (bSel % 0x40 == 0) return;
	if (bSel & KEY_1) { m_pMario[0].SetSelect(Player1); };
	if (bSel & KEY_2) { m_pMario[1].SetSelect(Player1); };
	if (bSel & KEY_3) { m_pMario[2].SetSelect(Player1); };
	if (bSel & KEY_4) { m_pMario[3].SetSelect(Player1); };
	if (bSel & KEY_5) { m_pMario[4].SetSelect(Player1); };
	if (bSel & KEY_6) { m_pMario[5].SetSelect(Player1); };

	bSel &= !(0x40 - 1);	// 0x63������ ��Ʈ�� ���� ��Ű�� �Է� Ű���� And ���� = ���� 6��Ʈ �ʱ�ȭ
	
}

void SceneManager::Destroy()
{
	if (m_pBlock != nullptr) delete[] m_pBlock;
	if (m_pWall != nullptr) delete[] m_pWall;
}

void SceneManager::ReadyToNextFrame()
{
	for (int i = 0; i < MaxMario; i++) {
		m_pMario[i].GetCollObjects().clear();	// �浹�� ��ü �ʱ�ȭ
		m_pMario[i].SetCollside(0);		// �浹�� ���� �ʱ�ȭ
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
	char* tmpbuf = buf;
	int   retval = 0;

	for (int i = 0; i < MaxMario; i++) {
		RecvMarioDataFormat mariodata = *((RecvMarioDataFormat*)&tmpbuf);

		m_pMario[i].SetMarioRecvData(mariodata);
		tmpbuf += sizeof(RecvMarioDataFormat);
		retval += sizeof(RecvMarioDataFormat);
	}

	RecvStageDataFormat stagedata = *((RecvStageDataFormat*)&tmpbuf);

	m_kKey.SetPosition(Vector2(stagedata.wKeyXPos, stagedata.wKeyYPos));
	m_kKey.SetKeystatus(stagedata.IsOpen);
	m_dDoor.SetOpen(stagedata.IsOpen);
	tmpbuf += sizeof(RecvMarioDataFormat);
	retval += sizeof(RecvMarioDataFormat);

	return retval;
}

/*
struct RecvMarioDataFormat {
WORD iMarioNum;
WORD iMarioPlayerNum;
WORD wxPos;
WORD wyPos;
bool bSelect;
bool bLookDirection;
WORD eSpriteState;
};

struct RecvStageDataFormat {
WORD wKeyXPos;
WORD wKeyYPos;
bool IsOpen;
};*/