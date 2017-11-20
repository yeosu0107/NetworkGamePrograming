#pragma once
#include "Scene.h"
#include "stdafx.h"

class FrameWork
{
private:
	Scene m_pScene[MaxStage];
public:
	FrameWork();
	~FrameWork();
	void Run();
};