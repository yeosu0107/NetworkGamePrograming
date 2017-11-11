#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <Windows.h>

using namespace std;

#define SERVERPORT 9000

struct MarioDataFormat {
	WORD	iMarioNum;				// 이 정보의 주인인 마리오 Number
	WORD	iMarioPlayerNum;	// 마리오를 할당 받은 Player식별 번호
	WORD	wxPos;					// 마리오의 X포지션
	WORD	wyPos;					// 마리오의 Y포지션
	bool	bSelect;						// 마리오의 선택 여부
	bool	bLookDirection;			// 보는 방향
	WORD 	eSpriteState;			// 마리오의 Sprite상태
};

struct StageDataFormat {
	UINT	wStageNum;				// 현재 스테이지 레벨
	WORD	wKeyXPos;				// 열쇠 오브젝트의 X좌표
	WORD	wKeyYPos;				// 열쇠 오브젝트의 Y좌표
	bool	IsOpen;						// 문이 열렸는 지에 대한 정보
};

struct SendDataFormat {
	MarioDataFormat m_Mario[6];	// 6개의 마리오에 대한 정보
	StageDataFormat m_Stage;		// 현재 스테이지에 대한 정보
};


extern void err_display(char *msg);

extern int recvn(SOCKET s, char *buf, int len, int flags);
