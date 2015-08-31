// 查询函数地址.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "windows.h"
int main(int argc, char* argv[])
{
	char Name[20];
	char File[20];
	DWORD addr;
	printf("Dll文件名：");
	scanf("%s",File);
	printf("\nAPI函数名：");
	scanf("%s",Name);
	printf("\n函数地址是：%0xH",
			GetProcAddress(LoadLibrary(File),Name));
	getchar();
	getchar();
	return 0;
}
