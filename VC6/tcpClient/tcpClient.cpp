// tcpClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "winsock2.h"

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
	WSADATA wd;
	char b[128] = {0};
	int l, nAddrLen;
	SOCKET s;
	sockaddr_in remoteAddr;

	WSAStartup(MAKEWORD(2, 2), &wd);			//初始化WSA库
	//创建套接字
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET  == s)
	{
		printf("Create socket error\n");
		goto ret;
	}
	//连接服务端
	remoteAddr.sin_family = AF_INET;
	remoteAddr.sin_port = htons(12121);
	remoteAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	if(SOCKET_ERROR == connect(s, (const sockaddr*)&remoteAddr, sizeof(remoteAddr)))
	{
		printf("Connet Failue!\n");
		goto ret;
	}
	printf("连接成功!\n");
	//向服务端发送数据
	strcpy(b, "Hello sever");
	send(s, b, 32, 0);
	//从服务端接收数据 
	recv(s, b, 32, 0);
	//打印数据 
	printf("收到消息：\n");
	printf(b);
	//退出
ret:
	getchar();
	return 0;
}
