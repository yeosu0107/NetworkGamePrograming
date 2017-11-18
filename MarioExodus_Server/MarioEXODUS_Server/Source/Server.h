#pragma once

DWORD WINAPI ClientThread(LPVOID arg);
DWORD WINAPI GameControlThread(LPVOID arg);

class ServerControl;
extern ServerControl* server;

class ServerControl
{
private:
	int m_NumOfClient;
	bool m_waitEvent;
	//클라이언트가 1개면 false, 2개면 true
public:
	ServerControl();
	~ServerControl();

	bool IsClientFull();

	void ClientDisconnect();

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
	char m_recvBuf[2];
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
};

