#pragma once
#include "Scene.h"

DWORD WINAPI ClientThread(LPVOID arg);
DWORD WINAPI GameControlThread(LPVOID arg);

class ServerControl;
extern ServerControl* server;

class ServerControl
{
private:
	int m_NumOfClient;
	bool m_waitEvent; //클라이언트가 1개면 false, 2개면 true

	//recv데이터 저장 버퍼
	WORD* m_RecvBufs[2];
	
	//게임 컨트롤
	Scene m_pScene[MaxStage];
	int m_iStageNum;
public:
	ServerControl();
	~ServerControl();

	bool IsClientFull();

	void ClientDisconnect();

	void getRecvDatas(int m_iClientNum, char* m_recvData);

	void ClearRecvBuf() {
		memset(m_RecvBufs, 0, sizeof(WORD*) * 2);
		m_RecvBufs[0] = nullptr;
		m_RecvBufs[1] = nullptr;
	}

	void PrintRecvBufs() {
		for (int i = 0; i < 2; ++i) {
			if (m_RecvBufs[i] != nullptr) {
				printf("%d ", i);
				printRecvData((char*)m_RecvBufs[i]);
			}
		}
	}

	void ApplyObjectsStatus();

	void ObjectsCollision();

	void ChangeSceneCheck();

	int getNumOfClient() const { return m_NumOfClient; }

	bool getWaitEvent() const { return m_waitEvent; }
};

const int SENDSIZE = 10;

class ClientControl
{
private:
	char* m_recvBuf;
	char m_sendBuf[SENDSIZE];

	SOCKET* m_socket;
	int m_ClientNum;
public:
	ClientControl(SOCKET* socket, int num);
	~ClientControl();

	int RecvKeyStatus();

	void DivideKey();

	void GetObjectsStatus();

	int SendObjectsStatus();

	int getClientNum() const { return m_ClientNum; }

	char* getRecvBuf() const { return m_recvBuf; }
};

