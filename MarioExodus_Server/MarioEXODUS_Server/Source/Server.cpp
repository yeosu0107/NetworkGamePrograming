#include "stdafx.h"
#include "Server.h"
#include <fstream>

HANDLE ClientRecvEvent[2];
HANDLE InteractiveEvent;
ServerControl* server;



//쓰레드 함수

DWORD WINAPI GameControlThread(LPVOID arg)
{
	//ServerControl* thisPoint = (ServerControl*)arg;

	while (1) {
		DWORD retval = WaitForMultipleObjects(2, ClientRecvEvent, server->getWaitEvent(), 400);
		//비정상적인 작동으로 이벤트가 set되지 않았을 때 멈추는 현상 방지를 위해 타임아웃 설정 (0.4초)
		ResetEvent(InteractiveEvent);
		
		//server->PrintRecvBufs();
		server->ObjectsCollision();
		server->ApplyObjectsStatus();
		server->ChangeSceneCheck();
		server->CombinationKeys();
		server->ClearBufs();

		SetEvent(InteractiveEvent);
	}
	return 0;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	ClientControl* thisPoint = (ClientControl*)arg;
	int ClientNum = thisPoint->getClientNum();
	DWORD status;
	while (1) {	
		if (thisPoint->RecvKeyStatus() == -1) {
			delete thisPoint;
			server->ClientDisconnect(ClientNum);
			break;
		}
		server->getRecvDatas(thisPoint->getClientNum(), thisPoint->getRecvBuf());
		SetEvent(ClientRecvEvent[thisPoint->getClientNum()]);
		status = WaitForSingleObject(InteractiveEvent, 400);
		if(status!=WAIT_FAILED)
			thisPoint->SendObjectsStatus(false);
		else
			thisPoint->SendObjectsStatus(true);
	}
	return 0;
}

//////////////////////////////////////////////////////////////////
//--------------------------------------------//
//-----------------ServerControl----------------//
//--------------------------------------------//
//////////////////////////////////////////////////////////////////

ServerControl::ServerControl()
{
	m_NumOfClient = 0;
	m_waitEvent = false;
	memset(m_recvBufs, 0, sizeof(WORD*) * 2);

	InteractiveEvent = CreateEvent(NULL, true, false, NULL);
	ClientRecvEvent[0] = CreateEvent(NULL, false, false, NULL);
	ClientRecvEvent[1] = CreateEvent(NULL, false, false, NULL);

	InitGameScene(); //게임 초기화

	ClientNum[0] = false;
	ClientNum[1] = false;
}

ServerControl::~ServerControl()
{

}

void ServerControl::InitGameScene()
{
	//게임 초기 세팅
	std::ifstream Input("InitialData.ini");

	Vector2 pMarioPos[MaxMario];
	Vector2 vDoorPos, vKeyPos;
	Vector2* pBlockPos = nullptr;
	Vector2* pWallPos = nullptr;

	int iBlockCount;
	int iWallCount;

	for (int i = 0; i < MaxStage; ++i) {
		for (int j = 0; j < MaxMario; ++j) {
			Input >> pMarioPos[j].x >> pMarioPos[j].y;
		}
		Input >> vDoorPos.x >> vDoorPos.y;
		Input >> vKeyPos.x >> vKeyPos.y;
		Input >> iBlockCount;

		if (iBlockCount > 0)
			pBlockPos = new Vector2[iBlockCount]();

		for (int j = 0; j < iBlockCount; j++) {
			Input >> pBlockPos[j].x >> pBlockPos[j].y;
		}

		Input >> iWallCount;
		if (iWallCount > 0)
			pWallPos = new Vector2[iWallCount]();

		for (int j = 0; j < iWallCount; j++) {
			Input >> pWallPos[j].x >> pWallPos[j].y;
		}

		m_pScene[i].InitScene(i, pMarioPos, vDoorPos, vKeyPos, iWallCount, pWallPos, iBlockCount, pBlockPos);

		if (pBlockPos != nullptr)	delete[](pBlockPos);
		pBlockPos = nullptr;

		if (pWallPos != nullptr)	delete[](pWallPos);
		pWallPos = nullptr;
	}

	Input.close();

	m_iStageNum = 0;
	memset(m_sendBuf, 0, sizeof(char)*MaxSendBuf);
}

bool ServerControl::IsClientFull()
{
	if (m_NumOfClient > 2)
		return false;

	m_NumOfClient += 1;
	if (m_NumOfClient == 2) {
		m_waitEvent = true;
	}

	return true;
}

void ServerControl::ClientDisconnect(int client)
{
	m_NumOfClient -= 1;
	m_waitEvent = false;

	ClientNum[client] = false;

	Mario* tmpMario = m_pScene[m_iStageNum].getMario();
	for (int i = 0; i < MaxMario; ++i) {
		if (tmpMario[i].getPlayerNum() == client)
			tmpMario[i].SetSelect(client); //클라이언트가 연결을 해재하면 현재 소유중이던 마리오의 소유권도 모두 해재
	}

	if (m_NumOfClient < 0)
		m_NumOfClient = 0;
}

void ServerControl::getRecvDatas(int m_iClientNum, char* m_recvData)
{
	m_recvBufs[m_iClientNum] = (WORD*)m_recvData;
}

void ServerControl::ApplyObjectsStatus()
{
	m_pScene[m_iStageNum].Update(0, m_recvBufs[0], m_recvBufs[1]);
}

void ServerControl::ObjectsCollision()
{
	m_pScene[m_iStageNum].CheckObjectsCollision(m_recvBufs[0], m_recvBufs[1]);
}

void ServerControl::ChangeSceneCheck()
{
	if (m_pScene[m_iStageNum].IsClear())
		m_iStageNum++;
	m_pScene[m_iStageNum].ReadyToNextFrame();
}

void ServerControl::CombinationKeys()
{
	WORD StageNum = m_iStageNum;
	MarioDataFormat marioData[6];
	StageDataFormat stageData;
	StageBlockFormat blockData[2];
	char* tmpBuf = m_sendBuf;

	for (int i = 0; i < MaxMario; ++i) {
		marioData[i] = m_pScene[m_iStageNum].getMario()[i].CombinationData();
	}
	stageData = m_pScene[m_iStageNum].getKey()->CombinationData();

	stageData.wStageNum = StageNum;

	for (int i = 0; i < m_pScene[m_iStageNum].getBlockCount(); ++i) {
		blockData[i] = m_pScene[m_iStageNum].getBlock()[i].CominationData();
	}

	memset(m_sendBuf, 0, sizeof(char)*MaxSendBuf);

	memcpy(tmpBuf, &stageData, sizeof(StageDataFormat));

	tmpBuf += sizeof(StageDataFormat);

	memcpy(tmpBuf, marioData, sizeof(MarioDataFormat) * 6);

	tmpBuf += sizeof(MarioDataFormat) * 6;

	memcpy(tmpBuf, blockData, sizeof(StageBlockFormat) * 2);

	memcpy(m_backupBuf, m_sendBuf, MaxSendBuf);
}

int ServerControl::getNumOfClient() {
	int client = -1;
	for (int i = 0; i < 2; ++i) {
		if (!ClientNum[i]) {
			ClientNum[i] = true;
			client = i;
			break;
		}
	}
	return client;
}

//////////////////////////////////////////////////////////////////
//--------------------------------------------//
//-----------------ClientControl-----------------//
//--------------------------------------------//
//////////////////////////////////////////////////////////////////

ClientControl::ClientControl(SOCKET* socket, int num)
{
	m_ClientNum = num;
	m_socket = socket;

	m_recvBuf = new char[2];

	memset(m_recvBuf, 0, sizeof(m_recvBuf));
	memset(m_sendBuf, 0, sizeof(m_sendBuf));
}

ClientControl::~ClientControl()
{
	closesocket(*m_socket);
}

int ClientControl::RecvKeyStatus()
{
	int retval = -1;
	retval = recvn(*m_socket, m_recvBuf, sizeof(WORD), 0);

	if (retval == SOCKET_ERROR) {
		err_display("recv()");
		memset(m_recvBuf, 0, sizeof(m_recvBuf));
		return -1;
	}
	return retval;
}



void ClientControl::GetObjectsStatus()
{
	
}

int ClientControl::SendObjectsStatus(bool backup)
{
	int retval = -1;
	if(!backup)
		retval = send(*m_socket, server->getSendData(), MaxSendBuf, 0);
	else
		retval = send(*m_socket, server->getBackupData(), MaxSendBuf, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	return 0;
}