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
		DWORD retval = WaitForMultipleObjects(2, ClientRecvEvent, server->getWaitEvent(), INFINITE);
		ResetEvent(InteractiveEvent);
		
		//server->PrintRecvBufs();
		server->ObjectsCollision();
		server->ApplyObjectsStatus();
		server->ChangeSceneCheck();
		server->ClearRecvBuf();

		SetEvent(InteractiveEvent);
	}
	return 0;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	ClientControl* thisPoint = (ClientControl*)arg;
	while (1) {
		if (thisPoint->RecvKeyStatus() == -1) {
			delete thisPoint;
			server->ClientDisconnect();
			break;
		}
		server->getRecvDatas(thisPoint->getClientNum(), thisPoint->getRecvBuf());
		SetEvent(ClientRecvEvent[thisPoint->getClientNum()]);
		WaitForSingleObject(InteractiveEvent, INFINITE);
		thisPoint->SendObjectsStatus();
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
	memset(m_RecvBufs, 0, sizeof(WORD*) * 2);

	InteractiveEvent = CreateEvent(NULL, true, false, NULL);
	ClientRecvEvent[0] = CreateEvent(NULL, false, false, NULL);
	ClientRecvEvent[1] = CreateEvent(NULL, false, false, NULL);

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
}

ServerControl::~ServerControl()
{

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

void ServerControl::ClientDisconnect()
{
	m_NumOfClient -= 1;
	m_waitEvent = false;


	if (m_NumOfClient < 0)
		m_NumOfClient = 0;
}

void ServerControl::getRecvDatas(int m_iClientNum, char* m_recvData)
{
	m_RecvBufs[m_iClientNum] = (WORD*)m_recvData;
	//printRecvData((char*)m_RecvBufs[m_iClientNum]);
}

void ServerControl::ApplyObjectsStatus()
{
	m_pScene[m_iStageNum].Update(0, m_RecvBufs[0], m_RecvBufs[1]);
}

void ServerControl::ObjectsCollision()
{
	m_pScene[m_iStageNum].CheckObjectsCollision(m_RecvBufs[0], m_RecvBufs[1]);
}

void ServerControl::ChangeSceneCheck()
{
	if (m_pScene[m_iStageNum].IsClear())
		m_iStageNum++;
	m_pScene[m_iStageNum].ReadyToNextFrame();
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
	DivideKey();
	return retval;
}

void ClientControl::DivideKey()
{
	//if(m_recvBuf & KEY_X)
}

void ClientControl::GetObjectsStatus()
{
	
}

int ClientControl::SendObjectsStatus()
{
	int retval = -1;
	retval = send(*m_socket, m_sendBuf, sizeof(WORD), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	return 0;
}


