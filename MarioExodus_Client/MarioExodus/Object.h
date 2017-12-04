#pragma once
#include "Vector2.h"
#include "Renderer.h"
#include <vector>

using namespace std;

class Object
{
public:
	static enum CollSide { CollLeft = 0x01, CollRight = 0x02, CollDown = 0x04, CollUp = 0x08 };

private:
	Vector2			m_vec2Pos;
	Vector2			m_vec2PrePos;
	Vector2			m_vec2Size;
	Renderer*		m_pRenderer;
	ObjectType		m_eObjectType;

public:
	Object();
	~Object();

public:
	void SetPosition(Vector2& pos);
	void SetPrePosition(Vector2& pos);
	void SetSize(Vector2& size) { m_vec2Size = size; }
	void SetRenderer(Renderer* pRenderer) { m_pRenderer = pRenderer; }
	void SetObjectType(ObjectType obt) { m_eObjectType = obt; }
	
	Vector2& GetPosition() { return m_vec2Pos; }			// 서버, 클라
	Vector2& GetPrePosition() { return m_vec2PrePos; }
	Vector2& GetSize() { return m_vec2Size; }				// 서버, 클라

	ObjectType GetObjectType() { return m_eObjectType; }

	Renderer* GetRenderer() const { return m_pRenderer; }	// 클라
};

