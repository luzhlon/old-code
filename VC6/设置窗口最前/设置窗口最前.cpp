// 设置窗口最前.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "windows.h"

int main(int argc, char* argv[])
{
	HWND hwnd;
	RECT rt;
	char str[256];
	printf("输入窗口标题：\n");
	scanf("%s",str);
	hwnd=FindWindow(NULL,str);
	if(!hwnd)
		printf("Not Find This Window!\n");
	GetWindowRect(hwnd,&rt);
	if(SetWindowPos(hwnd,HWND_TOPMOST,rt.left,rt.top,rt.right-rt.left,rt.bottom-rt.top,SWP_SHOWWINDOW))
		printf("Success!\n");
	else
		printf("Failue!\n");
	getchar();
	getchar();
	return 0;
}
