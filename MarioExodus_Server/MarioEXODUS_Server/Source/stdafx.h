#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <Windows.h>

using namespace std;

#define SERVERPORT 9000

const int MaxMario = 6;
const int MaxSendBuf = 75;
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

extern void printData(char *data);

#pragma pack(1)
struct MarioDataFormat {
	WORD	iMarioNum;				// 이 정보의 주인인 마리오 Number
	WORD	iMarioPlayerNum;	// 마리오를 할당 받은 Player식별 번호
	WORD	wxPos;					// 마리오의 X포지션
	WORD	wyPos;					// 마리오의 Y포지션
	bool	bSelect;						// 마리오의 선택 여부
	bool	bLookDirection;			// 보는 방향
	bool isExit;							// 문으로 나갔는지 여부


	MarioDataFormat() :
		iMarioNum(0), iMarioPlayerNum(0), wxPos(0), wyPos(0), bSelect(false), bLookDirection(false), isExit(false)
	{

	}
	MarioDataFormat(WORD num, WORD player, WORD x, WORD y, bool selected, bool dir, bool Exit)  :
		iMarioNum(num), iMarioPlayerNum(player), wxPos(x), wyPos(y), bSelect(selected), bLookDirection(dir), isExit(Exit)
	{ }
};
#pragma pack()

#pragma pack(1)
struct StageDataFormat {
	UINT	wStageNum;				// 현재 스테이지 레벨
	WORD	wKeyXPos;				// 열쇠 오브젝트의 X좌표
	WORD	wKeyYPos;				// 열쇠 오브젝트의 Y좌표
	bool	IsOpen;						// 문이 열렸는 지에 대한 정보

	StageDataFormat() :
		wStageNum(0), wKeyXPos(0), wKeyYPos(0), IsOpen(false)
	{ }

	StageDataFormat(UINT num, WORD x, WORD y, bool open) :
		wStageNum(num), wKeyXPos(x), wKeyYPos(y), IsOpen(open)
	{ }
};
#pragma pack()

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