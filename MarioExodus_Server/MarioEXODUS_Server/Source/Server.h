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

	//데이터 버퍼
	WORD* m_recvBufs[2];
	char		m_sendBuf[MaxSendBuf];
	
	//게임 컨트롤
	Scene m_pScene[MaxStage];
	int m_iStageNum;
public:
	ServerControl();
	~ServerControl();

	bool IsClientFull();

	void ClientDisconnect();

	void getRecvDatas(int m_iClientNum, char* m_recvData);
	char* getSendData() { return m_sendBuf; }

	void ClearBufs() {
		memset(m_recvBufs, 0, sizeof(WORD*) * 2);
		m_recvBufs[0] = nullptr;
		m_recvBufs[1] = nullptr;
	}

	void PrintBufs() {
		for (int i = 0; i < 2; ++i) {
			if (m_recvBufs[i] != nullptr) {
				printf("%d ", i);
				printData((char*)m_recvBufs[i]);
			}
		}
	}

	void ApplyObjectsStatus();

	void ObjectsCollision();

	void ChangeSceneCheck();

	void CombinationKeys();

	int getNumOfClient() const { return m_NumOfClient; }

	bool getWaitEvent() const { return m_waitEvent; }
};

class ClientControl
{
private:
	char* m_recvBuf;
	char m_sendBuf[MaxSendBuf];

	SOCKET* m_socket;
	int m_ClientNum;
public:
	ClientControl(SOCKET* socket, int num);
	~ClientControl();

	int RecvKeyStatus();

	void GetObjectsStatus();

	int SendObjectsStatus();

	int getClientNum() const { return m_ClientNum; }

	char* getRecvBuf() const { return m_recvBuf; }
};

