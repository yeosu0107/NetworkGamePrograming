#pragma once

class ServerControl
{
private:
	int m_NumOfClient;
	bool m_waitEvent;
public:
	ServerControl();
	~ServerControl();

	DWORD WINAPI GameControlThread(LPVOID arg);

	bool IsClientFull();

	void ClientDisconnect();

	void ApplyObjectsStatus();

	void ObjectsCollision();

	void ChangeSceneCheck();
};

class ClientControl
{
private:
	char* m_Buf;
	SOCKET* m_socket;
public:
	ClientControl(SOCKET* socket);
	~ClientControl();

	DWORD WINAPI ClientThread(LPVOID arg);

	int RecvKeyStatus();

	void DivideKey();

	void GetObjectsStatus();

	int SendObjectsStatus();
};