// Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
#include "Wincon.h"

typedef HWND (WINAPI *PROCGETCONSOLEWINDOW)();
PROCGETCONSOLEWINDOW GetConsoleWindow;

WNDPROC lpOldWndProc;

int _stdcall winproc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	printf("console window message:%x,%x,%x,%x\n",hwnd, message, wParam, lParam);
	CallWindowProc(lpOldWndProc,hwnd, message, wParam, lParam);
	return 0;
}

int main(int argc, char* argv[])
{
	char szTitle[128];

	HMODULE hKernel32 = GetModuleHandle("kernel32");
	GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32,"GetConsoleWindow");

	HWND hwnd=GetConsoleWindow();
	//GetConsoleTitle(szTitle,128);
	GetWindowText(hwnd,szTitle,128);

	lpOldWndProc=(WNDPROC)SetWindowLong(hwnd,GWL_WNDPROC,(long)winproc);
	
	printf("The Console window text is %s\n",szTitle);

	getchar();
	return 0;
}
