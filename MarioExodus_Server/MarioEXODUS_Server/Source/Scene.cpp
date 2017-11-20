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
	m_iNumOfClient = 0;

	m_Door.InitDoor(vDoorPos, false);
	m_Key.InitKey(vKeyPos, false);

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

void Scene::Update(float fElapsedTime, WORD* byInput)
{
	//������
	for (int i = 0; i < m_iNumOfClient; ++i) {
		SelectMario(i, byInput[i]);
		for (Mario& pMa : m_Mario)
			pMa.Update(i, fElapsedTime, byInput[i]);
	}
	//Ű
	m_Key.Update(fElapsedTime);
	//���
	if (m_pBlock != nullptr)
		for (int i = 0; i < m_iBlockCount; i++)
			m_pBlock[i].Update();
}

void Scene::CheckObjectsCollision()
{

}

void Scene::SelectMario(int iClient, WORD& bSel)
{
	//iClient <Player1=0, Player2=1>
	if (iClient > 1 || iClient < 0)
		return;

	if (bSel % 0x40 == 0) return;
	if (bSel & KEY_1) { m_Mario[0].SetSelect(iClient); };
	if (bSel & KEY_2) { m_Mario[1].SetSelect(iClient); };
	if (bSel & KEY_3) { m_Mario[2].SetSelect(iClient); };
	if (bSel & KEY_4) { m_Mario[3].SetSelect(iClient); };
	if (bSel & KEY_5) { m_Mario[4].SetSelect(iClient); };
	if (bSel & KEY_6) { m_Mario[5].SetSelect(iClient); };

	// 0x63������ ��Ʈ�� ���� ��Ű�� �Է� Ű���� And ���� = ���� 6��Ʈ �ʱ�ȭ
	bSel &= !(0x40 - 1);	
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