#pragma once
#include "stdafx.h"
#include "SceneManager.h"
#include "Time.h"
#include "Renderer.h"

class FrameWork
{
private:
	SceneManager	m_pScene[MaxStage];

	DWORD			m_dwInputSpecialkey;

	UINT			m_nStageNum;
	Time			m_tTime;
	Renderer*		m_pRenderer;

	bool			m_bIsPressKey;

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
};

