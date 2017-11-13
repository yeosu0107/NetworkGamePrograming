#pragma once
#include "stdafx.h"
#include "SceneManager.h"
#include "Time.h"
#include "Renderer.h"

class FrameWork
{
private:
	SceneManager	m_pScene[MaxStage];
	WSADATA			m_wsa;
	WORD 			m_wInputSpecialkey;
	SOCKET			m_sockServer;
	UINT			m_iStageNum;
	Time			m_tTime;
	Renderer*		m_pRenderer;

public:
	FrameWork();
	~FrameWork();

public:
	void Run();
	void SpecialKeyInput(int key, int x, int y);
	void SpecialKeyOutput(int key, int x, int y);
	void KeyInput(unsigned char key, int x, int y);
	void KeyOutput(unsigned char key, int x, int y);
	void InitFrameWork();
	void ReadyToNextFrame();
	int  ConnectServer();
	bool IsGameEnd();

public:
	int SendKeyStatus();
	int RecvObjectStatus(char* buf);
};

