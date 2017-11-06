#pragma once
#include "Object.h"

class BackGround : Object
{
	enum ObjectType{SkyNGround, Rogo, PressButton};
private:
	Object m_pObject[3];
	
	int m_nStage;

public:
	BackGround();
	BackGround(int stage, Renderer* pRend);
	~BackGround();

public:
	void Render();
	void InitBackGround(int stage, Renderer* pRend);
};

