#include "Subject.h"
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>

vector<Data> v;

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
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

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

//���� �б�
void FileOpen(char* name)
{
	ifstream in(name, ios::in |  ios::binary);
	if (!in) {
		printf("[%s]�� �������� �ʴ� �����Դϴ�.\n", name);
		exit(0);
	}
	printf("���� �б� ����\n");

	in.seekg(0, ios::end);
	int size = in.tellg();
	in.seekg(0, ios::beg);

	char tmp[BUFSIZE];
	memset(tmp,-1, BUFSIZE);

	v.push_back(Data(name, strlen(name)));
	v[0].setSize(size);
	/*while (!in.eof()) {
		memset(tmp, 0, BUFSIZE);
		in.read(tmp, BUFSIZE-1);
		v.push_back(Data(tmp));
	}*/
	while (!in.eof()) {
		in.read(tmp, BUFSIZE);
		v.push_back(Data(tmp, sizeof(tmp) / sizeof(char)));
		memset(tmp, -1, BUFSIZE);
	}

	//v.push_back(Data());
	
	in.close();
}


void DataSend(SOCKET& sock)
{
	int retval;

	// ������ ��ſ� ����� ����
	char buf[BUFSIZE];
	int len;

	vector<Data>::iterator itr = v.begin();


	//�۽� ������ ���� ����

	//������ ũ��(��������)
	len = itr->getSize();
	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	//������ �̸� ����(��������)
	len =strlen(itr->getData());
	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	//������ �̸� (��������)
	//strncpy(buf, itr->getData(), len);
	memcpy(buf, itr->getData(), len + 1);
	retval = send(sock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	itr++;

	while (itr != v.end()) {
		// ������ �Է�
		memset(buf, 0, sizeof(buf));
		len = itr->getSize();
		memcpy(buf, itr->getData(), len + 1);

		// ������ ������(���� ����)
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		// ������ ������(���� ����)
		retval = send(sock, buf, len, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}

		cout << "[TCP Ŭ���̾�Ʈ] " << sizeof(int) << "+" << retval << "����Ʈ�� ���½��ϴ�" << endl;
		itr++;
	}
}