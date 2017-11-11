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

	InteractiveEvent = CreateEvent(NULL, true, false, NULL);
	ClientRecvEvent[0] = CreateEvent(NULL, false, false, NULL);
	ClientRecvEvent[1] = CreateEvent(NULL, false, false, NULL);
}

ServerControl::~ServerControl()
{

}

DWORD WINAPI ServerControl::GameControlThread(LPVOID arg)
{
	DWORD retval = WaitForMultipleObjects(2, ClientRecvEvent, m_waitEvent, 400);

	ObjectsCollision();
	ApplyObjectsStatus();
	ChangeSceneCheck();

	SetEvent(InteractiveEvent);
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

ClientControl::ClientControl(SOCKET* socket)
{

}

ClientControl::~ClientControl()
{

}

DWORD WINAPI ClientControl::ClientThread(LPVOID arg)
{
	return 0;
}

int ClientControl::RecvKeyStatus()
{
	return 0;
}

void ClientControl::DivideKey()
{

}

void ClientControl::GetObjectsStatus()
{

}

int ClientControl::SendObjectsStatus()
{
	return 0;
}