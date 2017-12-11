#include "stdafx.h"
#include "FrameWork.h"
#include <fstream>

//#define APPLYTEST
CRITICAL_SECTION UpdateRenderCriticalSection;

DWORD WINAPI CommunicationServer(void* arg)
{
	int retval = 0;
	float fElapsedTime;	// 시간 갱신

	FrameWork* pFrameWork = (FrameWork*)arg;
	Time threadTime;

	while (retval != INVALID_SOCKET) {
		threadTime.Update(30.0f);

		retval = pFrameWork->SendKeyStatus();
		if (retval == INVALID_SOCKET)
			break;

		retval = pFrameWork->RecvObjectStatus();

		EnterCriticalSection(&UpdateRenderCriticalSection);
			pFrameWork->ApplySceneStatus();
			pFrameWork->Update();
			pFrameWork->ReadyToNextFrame();
		LeaveCriticalSection(&UpdateRenderCriticalSection);

		threadTime.Tock();
	}
	return 0;
}

FrameWork::FrameWork()
{
	InitFrameWork();
}


FrameWork::~FrameWork()
{
	delete(m_pRenderer);
	delete(m_pSound);

	for (int i = 0; i < MaxStage; i++)
		m_pScene[i].Destroy();

	if (m_hThreadHandle != NULL)
		CloseHandle(m_hThreadHandle);

	DeleteCriticalSection(&UpdateRenderCriticalSection);
	closesocket(m_sockServer);
	WSACleanup();
}

bool FrameWork::Run()
{
	m_tTime.Update(60.0f);
	
	EnterCriticalSection(&UpdateRenderCriticalSection);
		m_pScene[m_iStageNum].Render();			 // 렌더링
	LeaveCriticalSection(&UpdateRenderCriticalSection);

	m_tTime.Tock();		// 현재시간 -> 이전 시간으로 변경
	glutSetWindowTitle(m_tTime.GetFrameTime().c_str());	// 타이틀 메시지 변경

	return m_bRun;
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
	m_pSound = new SoundManager();
	
	m_bFirstFrame = true;
	m_hThreadHandle = NULL;
	m_pSound->Play(SoundType::BGMSound);
	m_bRun = true;

	InitializeCriticalSection(&UpdateRenderCriticalSection);

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

		m_pScene[i].InitSceneManager(i, pMarioPos, vDoorPos, vKeyPos, iWallCount, pWallPos, iBlockCount, pBlockPos, m_pRenderer, m_pSound);

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
	if (m_bFirstFrame)
		m_bFirstFrame = false;

	

	if (!m_pSound->IsPlaying(SoundType::StageClearSound) && !m_pSound->IsPlaying(SoundType::BGMSound))
		m_pSound->Play(SoundType::BGMSound);
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

		std::cout << "통신할 IP주소 : ";
		std::cin.get(IPbuf, sizeof(IPbuf));
		//std::cout << IPbuf << std::endl;
		std::cout << "통신할 포트번호 : ";
		std::cin >> clientPort;
		std::cin.get();

		clientAddr.sin_family = AF_INET;
		clientAddr.sin_addr.s_addr = inet_addr(IPbuf);
		clientAddr.sin_port = htons(clientPort);
#endif
		retval = connect(m_sockServer, (SOCKADDR*)&clientAddr, sizeof(clientAddr));
		if (retval == SOCKET_ERROR) std::cout << "연결에 실패했습니다. IP주소와 포트번호를 확인해주세요." << std::endl << std::endl;
	}
	
	m_hThreadHandle = CreateThread(NULL, 0, CommunicationServer, (void*)this, 0, NULL);
	return retval;
}

int FrameWork::SendKeyStatus()
{
	int retval;
	retval = send(m_sockServer, (char*)&m_wInputSpecialkey, sizeof(WORD), 0);

	if (retval == SOCKET_ERROR) {
		std::cout << "서버와의 연결이 끊겼습니다." << std::endl;
		m_bRun = false;
		return SOCKET_ERROR;
	}

	if (m_wInputSpecialkey % 64 > 0)
		m_wInputSpecialkey -= m_wInputSpecialkey % 64;

	return retval;
}

int FrameWork::RecvObjectStatus()
{
	::ZeroMemory(m_RecvBuf, MAX_BUF);
	int retval;

	retval = recv(m_sockServer, m_RecvBuf, MAX_BUF, 0);
	if (retval == INVALID_SOCKET) {
		std::cout << "서버와의 연결이 끊겼습니다." << std::endl;
		m_bRun = false;
		return INVALID_SOCKET;
	}
	m_pBufptr = m_RecvBuf;

	if (!m_bFirstFrame && m_iStageNum != *(WORD*)m_pBufptr) 
		m_pSound->Play(SoundType::StageClearSound);

	m_iStageNum = *(WORD*)m_pBufptr; // 현재 스테이지 레벨을 FrameWork단계에서 읽어온다.
	return retval;
}

void FrameWork::ApplySceneStatus()
{
	m_pScene[m_iStageNum].ApplyObjectsStatus(m_pBufptr);
}

void FrameWork::Update()
{
	float fElapsedTime = m_tTime.Tick();
	m_pScene[m_iStageNum].Update(fElapsedTime);	// 업데이트 , Move등이 여기서 호출
}
