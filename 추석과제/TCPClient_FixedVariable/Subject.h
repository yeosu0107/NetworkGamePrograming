#pragma once
#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#define BUFSIZE    60

using namespace std;

#pragma pack(1)
class Data {
private:
	int m_size;
	string m_data;
public:
	Data(int size, string data) : m_size(size) {
		m_data = data;
	}
	~Data() {}

	void PrintData() {
		std::cout << "[ size : "<<m_size<<" ] "<<m_data << std::endl;
	}

	int getSize() const { return m_size; }
	void setSize(int size) { m_size = size; }
	string getData() { return m_data; }
};

void err_quit(char *msg);
void err_display(char *msg);

void FileOpen(char* name);
void DataSend(SOCKET& sock);