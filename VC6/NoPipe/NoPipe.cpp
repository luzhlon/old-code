// NoPipe.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#include "winsock.h"

#pragma comment(lib, "ws2_32.lib")

SOCKET s;
char wMessage[1024*5];
char str[1024]={0};
HANDLE hInput,hOut;

DWORD WINAPI CMDThread( LPVOID lpParameter)
{
	DWORD nRead;
	unsigned long nBuf;
	char nl[]={13,10,13,10};
	INPUT_RECORD ir;
	

a:	ZeroMemory(str,1024);
	scanf("%s",str);
	//WriteFile((HANDLE)s,str,nRead,&nRead,0);
	send(s,str,15,0);
	send(s,nl,4,0);
	goto a;
	return 0;
}

int main(int argc, char* argv[])
{
	DWORD nRead;
	AllocConsole();

	hOut=GetStdHandle(STD_OUTPUT_HANDLE);
	hInput=GetStdHandle(STD_INPUT_HANDLE);
	unsigned long ThreadId;
	//初始化socket，并且监听端口
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if(WSAStartup(sockVersion, &wsaData) != 0)
        return 0;
	//设置socket属性
    //s=WSASocket(AF_INET, SOCK_STREAM,IPPROTO_TCP,0,0,0);
	s=socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4500);
	sin.sin_addr.S_un.S_addr =inet_addr( "127.0.0.1");
	
	connect(s, (sockaddr*)&sin, sizeof(sin));
	CreateThread(NULL,NULL,CMDThread,NULL,NULL,&ThreadId);
	Sleep(100);
	while(1)
	{
		recv(s,wMessage,5*1024,0);
		//ReadFile((HANDLE)s,wMessage,1024,&nRead,0);
		printf(wMessage);
	}
	return 0;
}
