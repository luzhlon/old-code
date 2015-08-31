// tcpExample.cpp : Defines the entry point for the console application.
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
	SOCKET s, n;
	sockaddr_in localAddr, remoteAddr;

	WSAStartup(MAKEWORD(2, 2), &wd);			//初始化WSA库
	//创建套接字
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(INVALID_SOCKET  == s)
	{
		printf("Create socket error\n");
		goto ret;
	}
	//绑定端口
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(12121);
	localAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if(SOCKET_ERROR == bind(s, (const sockaddr*)&localAddr, sizeof(localAddr)))
	{
		printf("Bind port error\n");
		goto ret;
	}
	//设置套接字为监听模式
	if(SOCKET_ERROR == listen(s, 5))
	{
		printf("Listen error\n");
		goto ret;
	}
	//等待客户端连接 
	nAddrLen = sizeof(remoteAddr);
	n = accept(s, (struct sockaddr*)&remoteAddr, &nAddrLen);
	printf("收到一个连接：\n");
	//接收客户端发来的数据
	l = recv(n, b, 32, 0);
	//打印数据
	printf("收到消息：\n");
	printf(b);
	//向客户端发送数据
	strcpy(b, "Hello client!");
	send(n, b, 32, 0);
	//退出
ret:
	getchar();
	return 0;
}
