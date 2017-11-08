#pragma once
#pragma warning (disable : 4819)


#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <Windows.h>
#include <string>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include "Dependencies\glew.h"
#include "Dependencies\freeglut.h"

#pragma comment(lib,"Winmm.lib")


// DEFINE
const int MaxMario = 6;

const UINT Screen_Width = 962;
const UINT Screen_Height = 690;
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

struct RecvMarioDataFormat {
	WORD iMarioNum;
	WORD iMarioPlayerNum;
	WORD wxPos;
	WORD wyPos;
	bool bSelect;
	bool bLookDirection;
	WORD eSpriteState;
};

struct RecvStageDataFormat {
	WORD wKeyXPos;
	WORD wKeyYPos;
	bool IsOpen;
};