#include "stdafx.h"
#include "Utility.h"

bool isLittleEndian()
{
	unsigned int a = 0x01020304;

	if ((int)*((char*)&a) == 4)
		return true;
	
	else
		return false;
	
}