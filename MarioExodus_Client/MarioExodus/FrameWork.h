#pragma once
#include "stdafx.h"
#include "SceneManager.h"
#include "Time.h"
#include "Renderer.h"
#include "SoundManager.h"

DWORD WINAPI CommunicationServer(void* arg);

class FrameWork
{
private:
	SceneManager	m_pScene[MaxStage];
	SoundManager*	m_pSound;
	WSADATA			m_wsa;
	WORD 			m_wInputSpecialkey;
	SOCKET			m_sockServer;
	UINT			m_iStageNum;
	Time			m_tTime;
	Renderer*		m_pRenderer;

	HANDLE			m_hThreadHandle;
	char			m_RecvBuf[MAX_BUF];
	char*			m_pBufptr = nullptr;

	bool			m_bFirstFrame;	// 게임 시작 후 첫 프레임인지에 대한 여부 : 서버로부터 받아온 데이터가 초기 정보와 다를 수 있어 예외를 주기 위한 변수
	bool			m_bRun;

public:
	FrameWork();
	~FrameWork();

public:
	bool Run();
	void SpecialKeyInput(int key, int x, int y);
	void SpecialKeyOutput(int key, int x, int y);
	void KeyInput(unsigned char key, int x, int y);
	void KeyOutput(unsigned char key, int x, int y);
	void InitFrameWork();
	void ReadyToNextFrame();
	void ApplySceneStatus();
	void Update();
	int  ConnectServer();

public:
	int SendKeyStatus();
	int RecvObjectStatus();
};

