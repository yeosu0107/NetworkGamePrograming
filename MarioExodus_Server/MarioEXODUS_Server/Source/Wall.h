#pragma once
#include "Object.h"

class Wall : public Object
{
public:
	Wall() {}
	~Wall() {}

public:
	void InitWall(Vector2 & vPos);
};

