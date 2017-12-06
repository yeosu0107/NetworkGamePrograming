#include "stdafx.h"
#include "FrameWork.h"
#include <fstream>

//#define APPLYTEST

FrameWork::FrameWork()
{
	InitFrameWork();
}


FrameWork::~FrameWork()
{
	delete(m_pRenderer);
	for (int i = 0; i < MaxStage; i++)
		m_pScene[i].Destroy();
	closesocket(m_sockServer);
	WSACleanup();
}

void FrameWork::Run()
{
	m_tTime.Update(60.0f);

	SendKeyStatus();
	RecvObjectStatus();
	ApplySceneStatus();
	if (m_pScene[m_iStageNum].IsClear()) 
		m_iStageNum++;	// �������� Ŭ���� Ȯ��
	
	float fElapsedTime = m_tTime.Tick();	// �ð� ����

	m_pScene[m_iStageNum].Update(fElapsedTime, m_wInputSpecialkey);	// ������Ʈ , Move���� ���⼭ ȣ��
	m_pScene[m_iStageNum].Render();			 // ������

	ReadyToNextFrame();
	m_tTime.Tock();		// ����ð� -> ���� �ð����� ����
	glutSetWindowTitle(m_tTime.GetFrameTime().c_str());	// Ÿ��Ʋ �޽��� ����
}

void FrameWork::SpecialKeyInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		m_wInputSpecialkey |= DIR_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		m_wInputSpecialkey |= DIR_RIGHT;
		break;
	}

}

void FrameWork::SpecialKeyOutput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		m_wInputSpecialkey ^= DIR_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		m_wInputSpecialkey ^= DIR_RIGHT;
		break;
	}

}

void FrameWork::KeyInput(unsigned char key, int x, int y)
{
	DWORD byte = 0;

	switch (key)
	{
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		m_wInputSpecialkey |= (DWORD)pow(2, (int)key - (int)'1');
		break;

	case 'c':
	case 'C':
		m_wInputSpecialkey |= KEY_C;
		break;

	case 'x':
	case 'X':
		m_wInputSpecialkey |= KEY_X;
		break;
	}
}

void FrameWork::KeyOutput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
		break;

	case 'c':
	case 'C':
		m_wInputSpecialkey ^= KEY_C;
		break;

	case 'x':
	case 'X':
		m_wInputSpecialkey ^= KEY_X;
		break;
	}
}

void FrameWork::InitFrameWork()
{
	std::ifstream Input("InitialData.ini");
	
	Vector2 pMarioPos[MaxMario];
	Vector2 vDoorPos, vKeyPos;
	Vector2* pBlockPos = nullptr;
	Vector2* pWallPos = nullptr;

	int iBlockCount;
	int iWallCount;

	m_pRenderer = new Renderer(Screen_Width, Screen_Height);

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

		m_pScene[i].InitSceneManager(i, pMarioPos, vDoorPos, vKeyPos, iWallCount, pWallPos, iBlockCount, pBlockPos, m_pRenderer);

		if (pBlockPos != nullptr)	delete[](pBlockPos);
		pBlockPos = nullptr;

		if (pWallPos != nullptr)	delete[](pWallPos);
		pWallPos = nullptr;
	}

	Input.close();
	
	m_iStageNum = 0;
	m_wInputSpecialkey = 0;
	
	ConnectServer();
}

void FrameWork::ReadyToNextFrame()
{
	if (m_wInputSpecialkey % 64 > 0)
		m_wInputSpecialkey -= m_wInputSpecialkey % 64;
}



int FrameWork::ConnectServer()
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0)
		return 1;

	SOCKADDR_IN clientAddr;
	u_short clientPort = 0;

	char IPbuf[16];
	int retval = SOCKET_ERROR;
	
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockServer == INVALID_SOCKET) error_quit("socket()");

	while (retval == SOCKET_ERROR) {

#if defined TEST
		::ZeroMemory(&IPbuf, sizeof(IPbuf));
		::ZeroMemory(&clientAddr, sizeof(clientAddr));

		clientAddr.sin_family = AF_INET;
		clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		clientAddr.sin_port = htons(9000);
#else
		::ZeroMemory(&IPbuf, sizeof(IPbuf));
		::ZeroMemory(&clientAddr, sizeof(clientAddr));

		std::cout << "����� IP�ּ� : ";
		fgets(IPbuf, sizeof(IPbuf), stdin);
		IPbuf[strlen(IPbuf) - 1] = '\0';

		std::cout << "����� ��Ʈ��ȣ : ";
		std::cin >> clientPort;

		clientAddr.sin_family = AF_INET;
		clientAddr.sin_addr.s_addr = inet_addr(IPbuf);
		clientAddr.sin_port = htons(clientPort);
#endif
		retval = connect(m_sockServer, (SOCKADDR*)&clientAddr, sizeof(clientAddr));
		if (retval == SOCKET_ERROR) error_quit("connect()");
	}

	return retval;
}

bool FrameWork::IsGameEnd()
{
	if (m_iStageNum > MaxStage)
		return true;

	else
		return false;
}

int FrameWork::SendKeyStatus()
{
	int retval;
	retval = send(m_sockServer, (char*)&m_wInputSpecialkey, sizeof(WORD), 0);

	if (retval == SOCKET_ERROR) {
		std::cout << "�������� ������ ������ϴ�." << std::endl;
		glutLeaveMainLoop();
		return SOCKET_ERROR;
	}
	return retval;
}

int FrameWork::RecvObjectStatus()
{
	::ZeroMemory(m_RecvBuf, MAX_BUF);
	int retval;

	retval = recv(m_sockServer, m_RecvBuf, MAX_BUF, 0);
	if (retval == INVALID_SOCKET) {
		std::cout << "�������� ������ ������ϴ�." << std::endl;
		glutLeaveMainLoop();
		return INVALID_SOCKET;
	}
	m_pBufptr = m_RecvBuf;

	m_iStageNum = *(WORD*)m_pBufptr; // ���� �������� ������ FrameWork�ܰ迡�� �о�´�.

	return retval;
}

void FrameWork::ApplySceneStatus()
{
	m_pScene[m_iStageNum].ApplyObjectsStatus(m_pBufptr);
}