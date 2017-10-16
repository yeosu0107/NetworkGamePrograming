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

BOOL gotoXY(int x,int y)
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

COORD getXY()
{
	COORD pos;
	CONSOLE_SCREEN_BUFFER_INFO buf;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &buf);
	pos.X = buf.dwCursorPosition.X;
	pos.Y = buf.dwCursorPosition.Y;
	return pos;
}

DWORD WINAPI DataRecv(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	SOCKADDR_IN clientaddr;
	char buf[BUFSIZE];
	int addrlen;
	int retval;
	
	File* file = nullptr;
	bool start = true;
	float percent = 0;
	float total = 0;
	int len;
	COORD pos;

	addrlen = sizeof(clientaddr);
	getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

	// 클라이언트와 데이터 통신
	while (1) {
		// 데이터 받기(고정 길이)
		retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0) {
			//수신 종료
			start = true;
			file->saveFile();
			delete file;
			break;
		}

		if (start) {
			start = false;
			percent = 0;
			total = len;

			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
			}
			memset(buf, 0, sizeof(buf));
			retval = recvn(client_sock, buf, len, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}

			file = new File(len, buf);
			cout << buf << "수신 시작" << endl;
			pos = getXY();
			continue;
		}

		// 데이터 받기(가변 길이)
		memset(buf, 0, sizeof(buf));
		retval = recvn(client_sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}

		percent += len;
		file->inputData(len, buf);
		gotoXY(pos.X, pos.Y);
		printf("%.1f%%  %d %d", percent / total * 100, pos.X, pos.Y);

	}

	closesocket(client_sock);
	printf("\n[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	return 0;
}