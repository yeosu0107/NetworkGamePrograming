#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <Windows.h>

using namespace std;

#define SERVERPORT 9000

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


extern void err_display(char *msg);

extern int recvn(SOCKET s, char *buf, int len, int flags);
