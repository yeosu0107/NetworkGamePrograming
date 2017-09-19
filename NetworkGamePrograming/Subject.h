#pragma once
#include <winsock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <vector>

void err_display(char* msg);
void err_quit(char *msg);

//����1
void print_socket(WSADATA& wsa);

//����2
BOOL IsLittleEndian();
BOOL IsBigEndian();

//����3
BOOL GetIPAddr(char* name, IN_ADDR* addr);
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen);
void input(char* name);
void GetIPAddr(addrinfo* ptr, std::vector<char*>& tmp);
//void GetIPAddr(IN_ADDR addr);