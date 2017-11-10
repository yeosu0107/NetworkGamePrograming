#pragma once
#include "Object.h"
#include "Renderer.h"

class Wall : public Object
{
public:
	Wall();
	~Wall();

public:
	void InitWall(Vector2 & vPos, Renderer* pRend);
	void Render();
};

