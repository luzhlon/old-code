// ¼ì²â´°¿Ú.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"

int main(int argc, char* argv[])
{
	SendMessage((HWND)0x4057E,WM_CLOSE,0,0);
	DestroyWindow((HWND)0x4057E);
	return 0;
}
