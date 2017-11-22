#include "stdafx.h"
#include "Server.h"

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

		server->ObjectsCollision();
		server->ApplyObjectsStatus();
		server->ChangeSceneCheck();

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

	InteractiveEvent = CreateEvent(NULL, true, false, NULL);
	ClientRecvEvent[0] = CreateEvent(NULL, false, false, NULL);
	ClientRecvEvent[1] = CreateEvent(NULL, false, false, NULL);
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

void ServerControl::ApplyObjectsStatus()
{

}

void ServerControl::ObjectsCollision()
{

}

void ServerControl::ChangeSceneCheck()
{

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
	for (int i = 0; i < SENDSIZE; ++i) {
		m_sendBuf[i] = 127;
	}

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

	std::cout << retval << std::endl;
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