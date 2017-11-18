#include "stdafx.h"
#include "Server.h"

HANDLE ClientRecvEvent[2];
HANDLE InteractiveEvent;

//////////////////////////////////////////////////////////////////
//--------------------------------------------//
//-----------------ServerThread----------------//
//--------------------------------------------//
//////////////////////////////////////////////////////////////////

ServerControl::ServerControl()
{
	m_NumOfClient = 0;
	m_waitEvent = false;

	InteractiveEvent = CreateEvent(NULL, false, false, NULL);
	ClientRecvEvent[0] = CreateEvent(NULL, false, false, NULL);
	ClientRecvEvent[1] = CreateEvent(NULL, false, false, NULL);
}

ServerControl::~ServerControl()
{

}

DWORD WINAPI ServerControl::GameControlThread(LPVOID arg)
{
	ServerControl* thisPoint = (ServerControl*)arg;

	while (1) {
		DWORD retval = WaitForMultipleObjects(2, ClientRecvEvent, thisPoint->m_waitEvent, INFINITE);

		thisPoint->ObjectsCollision();
		thisPoint->ApplyObjectsStatus();
		thisPoint->ChangeSceneCheck();

		SetEvent(InteractiveEvent);
	}
	return 0;
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
//-----------------ClientThread-----------------//
//--------------------------------------------//
//////////////////////////////////////////////////////////////////

ClientControl::ClientControl(SOCKET* socket, int num)
{
	m_ClientNum = num;
	m_socket = socket;

	memset(m_recvBuf, 0, sizeof(m_recvBuf));
	memset(m_sendBuf, 0, sizeof(m_sendBuf));
}

ClientControl::~ClientControl()
{
	closesocket(*m_socket);
}

DWORD WINAPI ClientControl::ClientThread(LPVOID arg)
{
	ClientControl* thisPoint = (ClientControl*)arg;
	while (1) {
		thisPoint->RecvKeyStatus();
		SetEvent(ClientRecvEvent[thisPoint->m_ClientNum]);
		WaitForSingleObject(InteractiveEvent, INFINITE);
		thisPoint->SendObjectsStatus();
	}
	return 0;
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