#include "Subject.h"

#define SERVERPORT 9000

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if(retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if(retval == SOCKET_ERROR) err_quit("listen()");

	// ������ ��ſ� ����� ����
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE+1];
	int len;

	while(1){
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if(client_sock == INVALID_SOCKET){
			err_display("accept()");
			break;
		}

		// ������ Ŭ���̾�Ʈ ���� ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		File* file = nullptr;
		bool start = true;
		float percent = 0;
		float total = 0;

		// Ŭ���̾�Ʈ�� ������ ���
		while(1){
			// ������ �ޱ�(���� ����)
			retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
			if(retval == SOCKET_ERROR){
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				//���� ����
				start = true;
				file->saveFile();
				delete file;
				break;
			}
			
			if (start) {

				start = false;
				percent = 0;
				total = len;

				retval = recvn(client_sock, (char *)&len, sizeof(int), 0);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
				}
				memset(buf, 0, sizeof(buf));
				retval = recvn(client_sock, buf, len, 0);
				if (retval == SOCKET_ERROR) {
					err_display("recv()");
					break;
				}

				file = new File(len, buf);
				cout << buf << "���� ����" << endl;
				continue;
			}

			// ������ �ޱ�(���� ����)
			memset(buf, 0, sizeof(buf));
			retval = recvn(client_sock, buf, len, 0);
			if(retval == SOCKET_ERROR){
				err_display("recv()");
				break;
			}
			else if (retval == 0) {
				break;
			}
			else if (file) {
				percent += len;
				file->inputData(len, buf);
			}

			//// ���� ������ ���
			//buf[retval] = '\0';
			//printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
			//	ntohs(clientaddr.sin_port), buf);

			printf("%.1f%%\r", percent / total * 100);
			Sleep(100);
		}

		// closesocket()
		closesocket(client_sock);
		printf("[TCP ����] Ŭ���̾�Ʈ ����: IP �ּ�=%s, ��Ʈ ��ȣ=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		
	}

	// closesocket()
	closesocket(listen_sock);

	// ���� ����
	WSACleanup();
	return 0;
}