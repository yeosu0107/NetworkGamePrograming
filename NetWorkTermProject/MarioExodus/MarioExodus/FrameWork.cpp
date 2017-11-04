#include "stdafx.h"
#include "FrameWork.h"

FrameWork::FrameWork()
{
	InitFrameWork();
}


FrameWork::~FrameWork()
{
	delete(m_pRenderer);
}

void FrameWork::Run()
{
	m_tTime.Update(60.0f);

	float fElapsedTime = m_tTime.Tick();

	m_pScene[m_nStageNum].Update(fElapsedTime, m_dwInputSpecialkey);
	m_pScene[m_nStageNum].CheckObjectCollision();

	m_pScene[m_nStageNum].Render();

	m_tTime.Tock();

	ReadyToNextFrame();

	glutSetWindowTitle(m_tTime.GetFrameTime().c_str());
}

void FrameWork::SpecialKeyInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		m_dwInputSpecialkey |= DIR_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		m_dwInputSpecialkey |= DIR_RIGHT;
		break;
	case GLUT_KEY_UP:
		m_dwInputSpecialkey |= DIR_UP;
		break;
	}

	if (m_dwInputSpecialkey != 0) m_bIsPressKey = true;
}

void FrameWork::SpecialKeyOutput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		m_dwInputSpecialkey ^= DIR_LEFT;
		break;
	case GLUT_KEY_RIGHT:
		m_dwInputSpecialkey ^= DIR_RIGHT;
		break;
	case GLUT_KEY_UP:
		m_dwInputSpecialkey ^= DIR_UP;
		break;
	}

	if (m_dwInputSpecialkey == 0) m_bIsPressKey = false;
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
		byte = (BYTE)key - (BYTE)'1';
		m_pScene[m_nStageNum].SelectMario(byte);
		break;

	case 'c':
	case 'C':
		break;
	}
}

void FrameWork::KeyOutput(unsigned char key, int x, int y)
{
}

void FrameWork::InitFrameWork()
{
	m_pRenderer = new Renderer(Screen_Width, Screen_Height);
	for(int i = 0; i < MaxStage; i++)
		m_pScene[i] = SceneManager(i, m_pRenderer);
	
	m_nStageNum = 0;
	m_dwInputSpecialkey = 0;
	m_dwInputKey = 0;
	m_bIsPressKey = false;

	m_tTime = Time();

}

void FrameWork::ReadyToNextFrame()
{
	m_dwInputKey = 0;
}
