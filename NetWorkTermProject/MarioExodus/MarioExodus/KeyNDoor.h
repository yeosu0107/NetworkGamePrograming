#pragma once
#include "Object.h"
#include "Mario.h"


class Door : public Object
{
private:
	bool		m_bOpen;

public:
	Door();
	Door(Renderer* rend);
	~Door();

public:
	void Render();
	void CollisionMario(Mario& other);
	
	void SetOpen(bool bOpen) { m_bOpen = bOpen; }
};


class Key : public Object
{
private:
	Mario*		m_pGrabMario;
	Object		m_oDish;

	bool		m_bOpen;

public:
	Key();
	Key(Renderer* rend);
	~Key();

public:
	void Render();
	void CollisionMario(Mario& other);
	void CollisionDoor(Door& other);
	void Update(float fTimeElapsed);
};