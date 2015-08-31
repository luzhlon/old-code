// SizeOfMain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"

int main(int argc, char* argv[])
{
	int asize;
	int address=(int)main;
	goto a;
c:	printf("MAIN的长度为：%d字节",asize);
	getchar();
	return 0;
a:
	_asm{
		call b
b:
		pop eax
		sub eax,address
		mov asize,eax
		}
	goto c;
}


