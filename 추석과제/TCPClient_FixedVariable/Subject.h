#pragma once
#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#define BUFSIZE    512

using namespace std;

#pragma pack(1)
class Data {
private:
	int m_size;
	char* m_data;
public:
	Data(char* data, int size){
		m_size = size;
		m_data = new char[m_size];
		memcpy(m_data, data, m_size+1);
	}
	Data() {
		m_data = nullptr;
		m_size = -1;
	}
	~Data() {}

	int getSize() const { return m_size; }
	void setSize(int size) { m_size = size; }
	char* getData() { return m_data; }
};

void err_quit(char *msg);
void err_display(char *msg);

void FileOpen(char* name);
void DataSend(SOCKET& sock);