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
	bool m_waitEvent; //Ŭ���̾�Ʈ�� 1���� false, 2���� true

	//������ ����
	WORD* m_recvBufs[2];
	char		m_sendBuf[MaxSendBuf];
	char		m_backupBuf[MaxSendBuf];
	
	//���� ��Ʈ��
	Scene m_pScene[MaxStage];
	int m_iStageNum;

	bool ClientNum[2];

	int resetGameTimer = 0;

public:
	ServerControl();
	~ServerControl();

	void InitGameScene();

	bool IsClientFull();

	void ClientDisconnect(int client);

	void getRecvDatas(int m_iClientNum, char* m_recvData);
	char* getSendData() { return m_sendBuf; }
	char* getBackupData() { return m_backupBuf; }

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

	int getNumOfClient();

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

	int SendObjectsStatus(bool backup);

	int getClientNum() const { return m_ClientNum; }

	char* getRecvBuf() const { return m_recvBuf; }
};

