#include "Subject.h"

void err_display(char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);

	printf("[%s] %s\n", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_quit(char *msg) {
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

//과제1
//--------------------------------------------------------
void print_socket(WSADATA& wsa) {
	printf("wVersion : %d.%d\n", HIBYTE(wsa.wVersion), LOBYTE(wsa.wVersion));  //윈속 버전, 2바이트. 1바이트 + 1바이트
	printf("wHighVersion : %d.%d\n", HIBYTE(wsa.wHighVersion), LOBYTE(wsa.wHighVersion)); //지원하는 최상위 윈속 버전
	printf("szDescription : %s\n", wsa.szDescription);  //소켓구현에 대한 설명 (회사정보)
	printf("szSystemStatus : %s\n", wsa.szSystemStatus); //구성 정보
}

//과제2
//--------------------------------------------------------
BOOL IsLittleEndian()
{
	u_short num = 0x1234;   //10진수 4660 /  2진수 0001 0010  0011 0100
	if ((u_char)num == 0x34) //10진수  52   /  2진수 0011 0100
		return true;
	else
		return false;
}

BOOL IsBigEndian()
{
	u_short num = 0x1234;   //10진수 = 4660
	if ((u_char)num != 0x34) // 10진수  52
		return true;
	else
		return false;
}

//과제3
//--------------------------------------------------------
BOOL GetIPAddr(char* name, IN_ADDR* addr)
{
	HOSTENT* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbynama()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET)
		return false;
	memcpy(addr, ptr->h_addr, ptr->h_length);
	return true;
}

BOOL GetDomainName(IN_ADDR addr, char* name, int namelen)
{
	HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	if (ptr == NULL) {
		err_display("gethostbyname()");
		return false;
	}
	if (ptr->h_addrtype != AF_INET)
		return false;
	strncpy(name, ptr->h_name, namelen);
	return true;
}

void GetDomainName(char* name)
{
	HOSTENT* ptr = gethostbyname(name);
	if (ptr == NULL) {
		err_display("gethostbynama()");
		return;
	}
	if (ptr->h_addrtype != AF_INET)
		return;
	printf("input : %s\n", ptr->h_name);

	printf("\naliases\n");
	for (int i = 0; ptr->h_aliases[i] != NULL; ++i) {
		printf("%s\n", ptr->h_aliases[i]);
	}
	printf("\naddr_list\n");
	char addr4_str[20];
	char addr6_str[40];
	for (int i = 0; ptr->h_addr_list[i] != NULL; ++i) {
		inet_ntop(AF_INET, ptr->h_addr_list[i], addr4_str, sizeof(addr4_str));
		printf("%s\n", addr4_str);
		
	}
	inet_ntop(AF_INET6, "2001:4998 : c : a06::2 : 4008", addr6_str, sizeof(addr6_str));
	printf("%s\n", addr6_str);
}

