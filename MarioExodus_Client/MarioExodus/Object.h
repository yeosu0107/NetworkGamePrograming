#pragma once
#include "Vector2.h"
#include "Renderer.h"

class Object
{
private:
	Vector2			m_vec2Pos;
	Vector2			m_vec2Size;
	Renderer*		m_pRenderer;

public:
	Object();
	~Object();

public:
	void SetPosition(Vector2& pos);
	void SetSize(Vector2& size) { m_vec2Size = size; }
	void SetRenderer(Renderer* pRenderer) { m_pRenderer = pRenderer; }

	Vector2& GetPosition() { return m_vec2Pos; }			// 서버, 클라
	Vector2& GetSize() { return m_vec2Size; }				// 서버, 클라

	bool Collision(Object& other);							// 서버

	Renderer* GetRenderer() const { return m_pRenderer; }	// 클라
};

