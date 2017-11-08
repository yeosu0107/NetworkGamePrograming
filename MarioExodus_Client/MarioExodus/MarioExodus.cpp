// MarioExodus.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Renderer.h"
#include "FrameWork.h"

FrameWork g_FrameWork;



void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	g_FrameWork.Run();

	glutSwapBuffers();
}

// Idle , RenderScene에서 업데이트를 하게 만든다.
void Idle(void)
{
	RenderScene();
}

void MouseInput(int button, int state, int x, int y)
{
}

void KeyInput(unsigned char key, int x, int y)
{
	g_FrameWork.KeyInput(key, x, y);
}

void KeyOutput(unsigned char key, int x, int y)
{
	g_FrameWork.KeyOutput(key, x, y);
}

void SpecialKeyInput(int key, int x, int y)
{
	g_FrameWork.SpecialKeyInput(key, x, y);
}

void SpecialKeyOutput(int key, int x, int y)
{
	g_FrameWork.SpecialKeyOutput(key, x, y);
}

void Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, Screen_Width, 0, Screen_Height, -1, 1);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(Screen_Width, Screen_Height);
	glutCreateWindow("MARIO EXODUS");

	glewInit();

	glutDisplayFunc(RenderScene);
	glutIdleFunc(Idle);
	glutKeyboardFunc(KeyInput);
	glutKeyboardUpFunc(KeyOutput);
	glutMouseFunc(MouseInput);
	glutReshapeFunc(Reshape);
	glutSpecialFunc(SpecialKeyInput);
	glutSpecialUpFunc(SpecialKeyOutput);
	glutMainLoop();

	return 0;
}

