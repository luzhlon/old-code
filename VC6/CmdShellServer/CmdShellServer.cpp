// CmdShellServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "windows.h"
#include "resource.h"
#include "Winsock2.h"
#pragma comment(lib,"ws2_32.lib")

char pbuf[2048];
SOCKET sCmd;
HWND hDlgMain;

BOOL AddTextToEdit(HWND hEdit,char *szTxt,BOOL bNextLine)
{
	static char NextLine[]={13,10,0};

	SendMessage(hEdit,EM_SETSEL,-2,-1);
	SendMessage(hEdit,EM_REPLACESEL,0,(long)szTxt);
	if(bNextLine)
		SendMessage(hEdit,EM_REPLACESEL,0,(long)NextLine);
	return 1;
}

DWORD WINAPI CmdThread(LPVOID lpParameter)
{
	WSADATA wsaData;

	WSAStartup(MAKEWORD(2,2),&wsaData);
	SOCKET  sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4503);
	sin.sin_addr.S_un.S_addr =INADDR_ANY;

	// 使套接字和本地地址绑定
	bind(sListen, (LPSOCKADDR)&sin, sizeof(sin));
	// 设置套接字进入监听模式
	listen(sListen, 5);
	sockaddr_in remoteAddr; 
    int nAddrLen = sizeof(remoteAddr);
	// 接受一个新连接
	sCmd=accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);
	while(TRUE)
	{
		ZeroMemory(pbuf,2048);
		int ret=recv(sCmd,pbuf,2048,0);
        if(ret>0)
		{
			AddTextToEdit(GetDlgItem(hDlgMain,IDC_SHOW),pbuf,0);
		}
		else
			break;
	}
	return 0;
}

int CALLBACK ProcDlgCmd(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD ThreadId;
	char *NextLine="\r\n";
	char str[1024];
	int len;

	switch(message)
	{
	case WM_INITDIALOG:
		hDlgMain=hwnd;
		//此线程专供传输CMD命令行信息
		CreateThread(NULL,NULL,CmdThread,hwnd,NULL,&ThreadId);
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			len=GetDlgItemText(hwnd,IDC_EDIT,str,1024);
			send(sCmd,str,strlen(str),0);
			send(sCmd,NextLine,strlen(NextLine),0);
			//AddTextToEdit(GetDlgItem(hwnd,IDC_SHOW),pbuf,1);
			SetDlgItemText(hwnd,IDC_EDIT,0);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd,0);
		break;
	default:
		return 0;
	}
	return 1;
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     PSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	DialogBoxParam(hInstance,(const char *)IDD_DLGMAIN,NULL,ProcDlgCmd,0);
	return 0;
}



