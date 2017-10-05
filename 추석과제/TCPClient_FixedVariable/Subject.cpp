#include "Subject.h"
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>

vector<Data> v;

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

//파일 읽기
void FileOpen(char* name)
{
	ifstream in(name, ios::in |  ios::binary);
	if (!in) {
		printf("[%s]는 존재하지 않는 파일입니다.\n", name);
		exit(0);
	}
	printf("파일 읽기 성공\n");

	in.seekg(0, ios::end);
	int size = in.tellg();
	in.seekg(0, ios::beg);

	char tmp[BUFSIZE];
	memset(tmp,-1, BUFSIZE);

	v.push_back(Data(name, strlen(name)));
	v[0].setSize(size);
	/*while (!in.eof()) {
		memset(tmp, 0, BUFSIZE);
		in.read(tmp, BUFSIZE-1);
		v.push_back(Data(tmp));
	}*/
	while (!in.eof()) {
		in.read(tmp, BUFSIZE);
		v.push_back(Data(tmp, sizeof(tmp) / sizeof(char)));
		memset(tmp, -1, BUFSIZE);
	}

	//v.push_back(Data());
	
	in.close();
}


void DataSend(SOCKET& sock)
{
	int retval;

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE];
	int len;

	vector<Data>::iterator itr = v.begin();


	//송신 데이터 정의 전송

	//데이터 크기(고정길이)
	len = itr->getSize();
	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	//데이터 이름 길이(고정길이)
	len =strlen(itr->getData());
	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	//데이터 이름 (가변길이)
	//strncpy(buf, itr->getData(), len);
	memcpy(buf, itr->getData(), len + 1);
	retval = send(sock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	itr++;

	while (itr != v.end()) {
		// 데이터 입력
		memset(buf, 0, sizeof(buf));
		len = itr->getSize();
		memcpy(buf, itr->getData(), len + 1);

		// 데이터 보내기(고정 길이)
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// 데이터 보내기(가변 길이)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		cout << "[TCP 클라이언트] " << sizeof(int) << "+" << retval << "바이트를 보냈습니다" << endl;
		itr++;
	}
}