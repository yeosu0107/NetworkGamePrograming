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
	Vector2			m_vec2Size;
	Renderer*		m_pRenderer;
	vector<pair<WORD, Object*>> m_vCObjPair;
	WORD m_wCollside;
	ObjectType		m_eObjectType;

public:
	Object();
	~Object();

public:
	void SetPosition(Vector2& pos);
	void SetSize(Vector2& size) { m_vec2Size = size; }
	void SetRenderer(Renderer* pRenderer) { m_pRenderer = pRenderer; }
	void SetObjectType(ObjectType obt) { m_eObjectType = obt; }
	void SetCollside(WORD wCollside) { m_wCollside = wCollside; }

	Vector2& GetPosition() { return m_vec2Pos; }			// 서버, 클라
	Vector2& GetSize() { return m_vec2Size; }				// 서버, 클라
	vector<pair<WORD, Object*>>& GetCollObjects() { return m_vCObjPair; }
	ObjectType GetObjectType() { return m_eObjectType; }

	WORD GetCollSide() { return m_wCollside; }

	bool Collision(Object& other);							// 서버
	void CollisionScreen();
	void AfterCollision();

	Renderer* GetRenderer() const { return m_pRenderer; }	// 클라
};

