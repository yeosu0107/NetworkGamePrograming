#pragma once
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

#define BUFSIZE    512

using namespace std;

#pragma pack(1)
class Data {
private:
	int m_size;
	char* m_data;
public:
	Data(int size, char* data) :m_size(size) {
		m_data = new char[m_size];
		memcpy(m_data, data, size + 1);

	}
	~Data() {}

	char* getData() const { return m_data; }
	int getSize() const { return m_size; }
};

class File {
private:
	vector<Data> v;
	int m_size;
	string m_name;
public:
	File(int size, string name) : m_size(size) {
		m_name = name;
	}
	~File() { }
	void inputData(int size, char* data);
	void saveFile();
};



void err_quit(char *msg);
void err_display(char *msg);
int recvn(SOCKET s, char *buf, int len, int flags);

BOOL gotoXY(int x, int y);
COORD getXY();

DWORD WINAPI DataRecv(LPVOID arg);