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

void input(char* name)
{
	printf("input : %s\n", name);
	addrinfo hints;
	addrinfo *result;

	memset(&hints, 0x00, sizeof(struct addrinfo));
	hints.ai_flags = AI_FQDN | AI_ALL; //도메인 이름을 반환, ipv4 & ipv6 주소를 모두 가져와라
	hints.ai_family = AF_UNSPEC; //ipv4 & ipv6
	hints.ai_socktype = SOCK_STREAM; //tcp를 사용
	
	std::vector<char*> namelist;
	
	printf("\nIP list\n");
	if (getaddrinfo(name, NULL, &hints, &result) == 0) //성공시 0을 리턴 (주소, 서비스|포트번호, 소켓유형, 반환값)
	{
		GetIPAddr(result, namelist);
	}
	else {
		printf("empty\n");
	}


	

	//별명
	printf("\naliases list\n");
	for (int i = 0; i < namelist.size(); ++i)
		printf("%s\n", namelist[i]);

}

void GetIPAddr(addrinfo* ptr, std::vector<char*>& namelist)
{
	sockaddr_in* tmp;
	sockaddr_in6* tmp6;
	char buf[20];
	char buf2[40];

	for (addrinfo* curr = ptr; curr != NULL; curr = curr->ai_next) {
		switch (curr->ai_family)
		{
		case AF_INET:
			tmp = (sockaddr_in*)curr->ai_addr;
			inet_ntop(curr->ai_family, &tmp->sin_addr, buf, sizeof(buf));  // (ipv4, 주소, 저장버퍼, 버퍼사이즈)
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
		if(curr->ai_canonname!=NULL)
			namelist.emplace_back(curr->ai_canonname);
	}
}