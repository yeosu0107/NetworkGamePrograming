#pragma comment(lib, "ws2_32")


#include "Subject.h"


int main(int argc, char* argv[])
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) //���� �ʱ�ȭ
		return 1;

	input("yahoo.com");
	printf("\n-------------------\n");
	input("www.naver.com");
	printf("\n-------------------\n");
	input("125.209.222.141");
	printf("\n-------------------\n");
	input("2001:4998:58:c02::a9");
	
		
	//IN_ADDR addr;
	//if (GetIPAddr(TESTNAME, &addr)) {
	//	printf("IP�ּ�(��ȯ ��) : %s\n", inet_ntoa(addr));

	//	char name[256];
	//	if (GetDomainName(addr, name, sizeof(name))) {
	//		printf("������ �̸�(��ȯ ��) : %s\n", name);
	//	}
	//}


	//SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0); //tcp���� ����
	//if (tcp_sock == INVALID_SOCKET)
	//	err_quit("socket()");

	//
	//closesocket(tcp_sock); //tcp ���� �ݱ�

	WSACleanup(); //���� ����
	return 0;
}