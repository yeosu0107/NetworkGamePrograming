#pragma comment(lib, "ws2_32")


#include "Subject.h"

using namespace std;




int main(int argc, char* argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) //윈속 초기화
		return 1;

	input("yahoo.com");

	//IN_ADDR addr;
	//if (GetIPAddr(TESTNAME, &addr)) {
	//	printf("IP주소(변환 후) : %s\n", inet_ntoa(addr));

	//	char name[256];
	//	if (GetDomainName(addr, name, sizeof(name))) {
	//		printf("도메인 이름(변환 후) : %s\n", name);
	//	}
	//}


	//SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0); //tcp소켓 생성
	//if (tcp_sock == INVALID_SOCKET)
	//	err_quit("socket()");

	//
	//closesocket(tcp_sock); //tcp 소켓 닫기

	WSACleanup(); //윈속 종료
	return 0;
}