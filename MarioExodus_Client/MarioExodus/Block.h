#pragma once
#include "Object.h"
#include "Renderer.h"

class Block : public Object
{
private:
	int m_iValocity;
	int m_iXDir;
	int m_iYDir;

public:
	Block();
	~Block();

public:
	void InitBlock(Vector2 & vPos, Renderer* pRend);
	void Render();
	void AddDir(const int x) { m_iXDir += x; }
	void SetXDir(const int x) { m_iXDir = x; }
	void SetYDir(const int y) { m_iYDir = y; }
};

