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

void input()
{
	char name[50]="yahoo.com";
	addrinfo hints;
	addrinfo *result;
	//printf("입력 : ");
	//scanf("%s", &name);

	memset(&hints, 0x00, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_flags = AI_CANONNAME;
	
	
	//ipv4
	printf("\nipv4 list\n");
	if (getaddrinfo(name, "http", &hints, &result) == 0) //성공시 0을 리턴
	{
		GetIPAddr(result);
	}
	//ipv6
	printf("\nipv6 list\n");
	hints.ai_family = AF_INET6;
	if (getaddrinfo(name, "http", &hints, &result) == 0) //성공시 0을 리턴
	{
		GetIPAddr(result);
	}
	else {
		printf("empty\n");
	}
	//별명
	printf("\naliases list\n");
	HOSTENT* host = gethostbyname(name);
	int i;
	for (i = 0; host->h_aliases[i] != NULL; ++i) {
		printf("%s\n", host->h_aliases[i]);
	}
	if (i == 0)
		printf("empty\n");
	printf("\n");
}

void GetIPAddr(addrinfo* ptr)
{
	sockaddr_in* tmp;
	sockaddr_in6* tmp6;
	char buf[40];
	char buf2[60];

	for (addrinfo* curr = ptr; curr != NULL; curr = curr->ai_next) {
		switch (curr->ai_family)
		{
		case AF_INET:
			tmp = (sockaddr_in*)curr->ai_addr;
			inet_ntop(curr->ai_family, &tmp->sin_addr, buf, sizeof(buf));
			printf("%s\n", buf);
			break;
		case AF_INET6:
			tmp6 = (sockaddr_in6*)curr->ai_addr;
			inet_ntop(curr->ai_family, &tmp6->sin6_addr, buf2, sizeof(buf2));
			printf("%s\n", buf2);
			break;
		default:
			printf("error\n");
		}

	}

	
	/*printf("\naliases\n");
	for (int i = 0; ptr->h_aliases[i] != NULL; ++i) {
		printf("%s\n", ptr->h_aliases[i]);
	}
	printf("\naddr_list\n");
	char addr4_str[20];

	for (int i = 0; ptr->h_addr_list[i] != NULL; ++i) {
		inet_ntop(AF_INET, ptr->h_addr_list[i], addr4_str, sizeof(addr4_str));
		printf("%s\n", addr4_str);
	}*/
}


