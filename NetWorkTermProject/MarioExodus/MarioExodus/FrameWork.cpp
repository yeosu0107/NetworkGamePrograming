#include "stdafx.h"
#include "FrameWork.h"
#include <math.h>

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

	if (m_pScene[m_iStageNum].IsClear()) m_iStageNum++;	// 스테이지 클리어 확인
	
	float fElapsedTime = m_tTime.Tick();	// 시간 갱신

	m_pScene[m_iStageNum].Update(fElapsedTime, m_dwInputSpecialkey);	// 업데이트 , Move등이 여기서 호출
	m_pScene[m_iStageNum].CheckObjectCollision(m_dwInputSpecialkey);	// 충돌체크
	m_pScene[m_iStageNum].Render();										// 렌더링

	m_tTime.Tock();		// 현재시간 -> 이전 시간으로 변경

	ReadyToNextFrame();	

	glutSetWindowTitle(m_tTime.GetFrameTime().c_str());	// 타이틀 메시지 변경
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
	}

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
		m_dwInputSpecialkey |= (DWORD)pow(2, (int)key - (int)'1');
		break;

	case 'c':
	case 'C':
		m_dwInputSpecialkey |= KEY_C;
		break;

	case 'x':
	case 'X':
		m_dwInputSpecialkey |= KEY_X;
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
		m_dwInputSpecialkey ^= KEY_C;
		break;

	case 'x':
	case 'X':
		m_dwInputSpecialkey ^= KEY_X;
		break;
	}
}

void FrameWork::InitFrameWork()
{
	m_pRenderer = new Renderer(Screen_Width, Screen_Height);
	for(int i = 0; i < MaxStage; i++)
		m_pScene[i] = SceneManager(i, m_pRenderer);
	
	m_iStageNum = 0;
	m_dwInputSpecialkey = 0;

	m_tTime = Time();

}

void FrameWork::ReadyToNextFrame()
{
}
