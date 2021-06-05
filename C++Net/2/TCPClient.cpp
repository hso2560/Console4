#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WS2tcpip.h>
#include<iostream>
#include<WinSock2.h>
#include<string>
#pragma comment(lib,"ws2_32")
using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[오류발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddress;
	int serverPort = 9800;
	char received[256];
	string sent;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock을초기화합니다.
		ShowErrorMessage("WSAStartup()");
	clientSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓을생성합니다.
	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // 문자열IP를네트워크바이트형식으로
	serverAddress.sin_port = htons(serverPort); // 2바이트정수네트워크바이트형식으로

	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()");
	cout << "[현재상태] connect()\n";
	while (1) { // 반복적으로 서버로 메시지를 전송하고 에코메시지를 전달받음
		cout << "[메시지전송]: ";
		getline(cin, sent);
		if (sent == "") continue;
		send(clientSocket, sent.c_str(), sent.length(), 0);
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = '\0';
		if (strcmp(received, "[exit]") == 0) {
			cout << "[서버종료]\n";
			break;
		}
		cout << "[서버메시지]: " << received << '\n';
	}
	closesocket(clientSocket);
	WSACleanup();
	system("pause");
	return 0;
}