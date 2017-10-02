#include "Subject.h"
#include <vector>
#include <fstream>

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
	ifstream in(name);
	if (!in) {
		printf("[%s]�� �������� �ʴ� �����Դϴ�.\n", name);
		exit(0);
	}
	printf("���� �б� ����\n");
	
	string tmp;
	int size = 0;

	//in.seekg(0, ios::end);
	//size = in.tellg();
	//in.seekg(0, ios::beg);

	v.push_back(Data(0, name));
	while (getline(in, tmp)) {
		//tmp += '\r\n';
		v.push_back(Data(tmp.size(), tmp));
		size += tmp.size();
	}
	v[0].setSize(size);
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
	len = itr->getData().size();
	retval = send(sock, (char *)&len, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	//������ �̸� (��������)
	strncpy(buf, itr->getData().c_str(), len);
	retval = send(sock, buf, len, 0);
	if (retval == SOCKET_ERROR) {
		err_display("send()");
	}
	itr++;

	while (itr != v.end()) {
		// ������ �Է�
		memset(buf, 0, sizeof(buf));
		len = itr->getSize();
		strncpy(buf, itr->getData().c_str(), len);

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