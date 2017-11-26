#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <Windows.h>

using namespace std;

#define SERVERPORT 9000

const int MaxMario = 6;
const enum ObjectType { MARIO, BLOCK, WALL, KEY, DOOR };
const enum PlayerNumber { Player1, Player2 };

const UINT Screen_Width = 1280;
const UINT Screen_Height = 720;
const UINT MaxStage = 4;

const DWORD KEY_1 = 0x01;
const DWORD KEY_2 = 0x02;
const DWORD KEY_3 = 0x04;
const DWORD KEY_4 = 0x08;
const DWORD KEY_5 = 0x10;
const DWORD KEY_6 = 0x20;
const DWORD DIR_LEFT = 0x40;
const DWORD DIR_RIGHT = 0x80;
const DWORD KEY_C = 0x100;
const DWORD KEY_X = 0x200;

extern void err_display(char *msg);

extern int recvn(SOCKET s, char *buf, int len, int flags);

extern void printRecvData(char *data);

struct MarioDataFormat {
	WORD	iMarioNum;				// �� ������ ������ ������ Number
	WORD	iMarioPlayerNum;	// �������� �Ҵ� ���� Player�ĺ� ��ȣ
	WORD	wxPos;					// �������� X������
	WORD	wyPos;					// �������� Y������
	bool	bSelect;						// �������� ���� ����
	bool	bLookDirection;			// ���� ����
	WORD 	eSpriteState;			// �������� Sprite����
};

struct StageDataFormat {
	UINT	wStageNum;				// ���� �������� ����
	WORD	wKeyXPos;				// ���� ������Ʈ�� X��ǥ
	WORD	wKeyYPos;				// ���� ������Ʈ�� Y��ǥ
	bool	IsOpen;						// ���� ���ȴ� ���� ���� ����
};

struct SendDataFormat {
	MarioDataFormat m_Mario[6];	// 6���� �������� ���� ����
	StageDataFormat m_Stage;		// ���� ���������� ���� ����
};

class Vector2
{
public:
	int x, y;

public:
	Vector2() {
		x = y = 0;
	}
	Vector2(int _x, int _y) :x(_x), y(_y) {	}
	Vector2(Vector2&& other) { };
	Vector2(const Vector2& other) {
		x = other.x;
		y = other.y;
	}
	Vector2(const Vector2* other) {
		x = other->x;
		y = other->y;
	}
	~Vector2() { }

public:
	Vector2 GetVector2() { return Vector2(x, y); }

	int GetX() const { return x; }
	int GetY() const { return y; }

	void SetX(const int otherx) { x = otherx; }
	void SetY(const int othery) { y = othery; }

public:
	Vector2 operator=(const Vector2& other) {
		x = other.x; y = other.y;
		return Vector2(x, y);
	}

	Vector2 operator+(const Vector2& other) {
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(const Vector2& other) {
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 operator+=(const Vector2& other) {
		x += other.x; y += other.y;
		return Vector2(x, y);
	}

	Vector2 operator-=(const Vector2& other) {
		x -= other.x; y -= other.y;
		return Vector2(x, y);;
	}

	Vector2 operator/(const int value) {
		x /= value; y /= value;
		return Vector2(x, y);
	}

	Vector2 operator*(const int value) {
		x *= value; y *= value;
		return Vector2(x, y);
	}
};