#pragma once
#include "Object.h"
#include "Mario.h"


class Door : public Object
{
private:
	bool		m_bOpen;

public:
	Door();
	~Door();

public:
	void InitDoor(Vector2& vPos, bool bOpen);
	bool CollisionMario(Mario& other);
	
	void SetOpen(bool bOpen) { m_bOpen = bOpen; }
};


class Key : public Object
{
private:
	Mario*		m_pGrabMario;
	Object		m_oDish;

	bool		m_bUsedKey;

public:
	Key();
	~Key();

public:
	void InitKey(Vector2& vPos, bool bUsedKey);
	void CollisionMario(Mario& other);
	void CollisionDoor(Door& other);
	void Update(float fTimeElapsed);
	void SetMarioPtr(Mario * pMario);

	void SetKeystatus(bool bKeystatus) { m_bUsedKey = bKeystatus; }

	StageDataFormat CombinationData();
};