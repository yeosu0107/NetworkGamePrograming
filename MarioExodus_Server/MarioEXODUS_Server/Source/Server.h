#pragma once

class ServerControl
{
private:
	int m_NumOfClient;
	bool m_waitEvent;
public:
	ServerControl();
	~ServerControl();

	static DWORD WINAPI GameControlThread(LPVOID arg);

	bool IsClientFull();

	void ClientDisconnect();

	void ApplyObjectsStatus();

	void ObjectsCollision();

	void ChangeSceneCheck();

	int getNumOfClient() const { return m_NumOfClient; }
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

	static DWORD WINAPI ClientThread(LPVOID arg);

	int RecvKeyStatus();

	void DivideKey();

	void GetObjectsStatus();

	int SendObjectsStatus();
};