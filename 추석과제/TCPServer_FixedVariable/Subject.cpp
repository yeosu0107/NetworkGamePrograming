#include "Subject.h"

#include <fstream>
#include <Windows.h>

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}



void File::inputData(int size, char* data)
{
	v.emplace_back(Data(size, data));
}

void File::saveFile()
{
	ofstream out(m_name, ios::out | ios::binary);
	vector<Data>::iterator itr = v.begin();
	if(out)
	while (itr != v.end()) {
		out.write(itr->getData(), itr->getSize());
		itr++;
	}
	out.close();
}

void gotoXY(int x, int y) {

	COORD pos = { x - 1, y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}