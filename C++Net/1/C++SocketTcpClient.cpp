#include<WS2tcpip.h>
#include<iostream>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32")
#define PORT 4400
#define PACKET_SIZE 1024
using namespace std;

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	//tAddr.sin_addr.s_addr = inet_pton(AF_INET,"127.0.0.1",&tAddr.sin_addr);
	tAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //������Ʈ�� ��Ŭ�� �� �Ӽ��� ���� C/C++�� �Ϲ��� SDL�˻縦 �ƴϿ�� �ٲٸ� ��.

	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

	char cMsg[] = "Client Send";
	send(hSocket, cMsg, strlen(cMsg), 0);

	char cBuffer[PACKET_SIZE] = {};
	recv(hSocket, cBuffer, PACKET_SIZE, 0);

	printf("Received Message : %s\n", cBuffer);

	closesocket(hSocket);

	WSACleanup();
}