#include "Subject.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

int main(int argc, char *argv[])
{
	int retval;
	char filename[10];
	cout << "파일 명 입력 : ";
	cin >> filename;
	FileOpen(filename);
	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("connect()");

	//데이터보내기
	DataSend(sock);

	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}