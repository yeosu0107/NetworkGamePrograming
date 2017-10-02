#include "Subject.h"
#include <vector>
#include <fstream>

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
	ifstream in(name);
	if (!in) {
		printf("[%s]는 존재하지 않는 파일입니다.\n", name);
		exit(0);
	}
	printf("파일 읽기 성공\n");
	
	string tmp;
	int size = 0;

	//in.seekg(0, ios::end);
	//size = in.tellg();
	//in.seekg(0, ios::beg);

	v.push_back(Data(0, name));
	while (getline(in, tmp)) {
		//tmp += '\r\n';
		v.push_back(Data(tmp.size(), tmp));
		size += tmp.size();
	}
	v[0].setSize(size);
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
	len = itr->getData().size();
	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	//데이터 이름 (가변길이)
	strncpy(buf, itr->getData().c_str(), len);
	retval = send(sock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	itr++;

	while (itr != v.end()) {
		// 데이터 입력
		memset(buf, 0, sizeof(buf));
		len = itr->getSize();
		strncpy(buf, itr->getData().c_str(), len);

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