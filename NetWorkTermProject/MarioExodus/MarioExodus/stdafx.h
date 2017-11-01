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
const UINT Screen_Width = 962;
const UINT Screen_Height = 690;
const UINT MaxStage = 6;

const DWORD DIR_LEFT = 0x01;
const DWORD DIR_RIGHT = 0x02;
const DWORD DIR_UP = 0x04;

typedef enum TextureNumber { Mario_Sprite, Mario_None, Marios_Number, BackSky,
							Block, Door, Key, Key_Dish, Main_Rogo, Press_C } TextureNumber;