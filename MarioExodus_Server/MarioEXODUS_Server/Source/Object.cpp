#include "stdafx.h"
#include "Object.h"
#include "Block.h"
#include <algorithm>


Object::Object()
{
	m_wCollside = 0;
}


Object::~Object()
{
}

void Object::SetPosition(Vector2 & pos)
{
	m_vec2Pos = pos;
}

bool Object::Collision(Object & other)
{
	WORD wCollside = 0;

	Vector2 memberBeg(GetPosition().x - GetSize().x / 2, GetPosition().y - GetSize().y / 2);
	Vector2 memberEnd(GetPosition().x + GetSize().x / 2, GetPosition().y + GetSize().y / 2);
	Vector2 otherBeg(other.GetPosition().x - GetSize().x / 2, other.GetPosition().y - GetSize().y / 2);
	Vector2 otherEnd(other.GetPosition().x + GetSize().x / 2, other.GetPosition().y + GetSize().y / 2);

	// 충돌 X시 여부 거름
	if (memberBeg.x > otherEnd.x) return false;
	if (memberBeg.y > otherEnd.y) return false;
	if (memberEnd.x < otherBeg.x) return false;
	if (memberEnd.y < otherBeg.y) return false;

	// 충돌 할 시 충돌 한 위치와 오브젝트에 대한 정보를 저장함
	int distanceX = m_vec2Pos.x - other.m_vec2Pos.x;
	int distanceY = m_vec2Pos.y - other.m_vec2Pos.y;
	
	if( distanceX * distanceX <= distanceY * distanceY ) {
		if (distanceY < 0) 	wCollside |= CollUp; 
		else				wCollside |= CollDown;
			
	}
	else if (distanceX * distanceX > distanceY * distanceY) {
		if (distanceX < 0)	wCollside |= CollRight; 
		else				wCollside |= CollLeft;
	}
	

	m_wCollside |= wCollside;

	pair<WORD, Object*> pairWordObjectp(wCollside, &other);
	m_vCObjPair.emplace_back(pairWordObjectp);
	
	return true;
}

void Object::CollisionScreen()
{
	int iGround = 170;
	WORD wCollside = GetCollSide();

	if (GetPosition().x <= GetSize().x / 2) {
		wCollside |= CollLeft;
		SetPosition(Vector2(GetSize().x / 2, GetPosition().y));
	}
	else if (GetPosition().x >= Screen_Width - GetSize().x / 2) {
		wCollside |= CollRight;
		SetPosition(Vector2(Screen_Width - GetSize().x / 2, GetPosition().y));
	}

	if (GetPosition().y <= iGround + GetSize().y / 2) {
		wCollside |= CollDown;
		SetPosition(Vector2(GetPosition().x, iGround + GetSize().y / 2));
	}
	else if (GetPosition().y >= Screen_Height - GetSize().y / 2) {
		wCollside |= CollUp;
		SetPosition(Vector2(GetPosition().x, Screen_Height - GetSize().y / 2));
	}

	SetCollside(wCollside);
}

void Object::AfterCollision()
{
	WORD wCollside = GetCollSide();
	vector<pair<WORD, Object*>> vCollObjects = GetCollObjects();

	if (wCollside & CollDown) {
		auto p = find_if(vCollObjects.begin(), vCollObjects.end(), [](pair<WORD, Object*>& a) {
			return a.first == (WORD)CollDown;
		});
		if (p != vCollObjects.end()) {
			SetPosition(Vector2(GetPosition().x, (*p).second->GetPosition().y + GetSize().y));
		}
	}

	if (wCollside & CollRight) {
		auto p = find_if(vCollObjects.begin(), vCollObjects.end(), [](pair<WORD, Object*>& a) {
			return a.first == (WORD)CollRight;
		});
		if (p != vCollObjects.end() && (*p).second->GetObjectType() == BLOCK) {
			((Block*)((*p).second))->AddDir(+1);
		}
	}

	if (wCollside & CollLeft) {
		auto p = find_if(vCollObjects.begin(), vCollObjects.end(), [](pair<WORD, Object*>& a) {
			return a.first == (WORD)CollLeft;
		});
		if (p != vCollObjects.end() && (*p).second->GetObjectType() == BLOCK) {
			((Block*)((*p).second))->AddDir(-1);
		}
	}
}
