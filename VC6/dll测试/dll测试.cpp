// dll测试.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include "stdio.h"

typedef (CALLBACK * lpFunAddr)(char *,char *);

int main(int argc, char* argv[])
{
	
	HINSTANCE h=LoadLibrary("测试.dll");
	lpFunAddr pCal=(lpFunAddr)GetProcAddress(h,"calculate");
	char str[1024];
	char result[15];
	printf("请输入算式：");
	scanf("%s",str);
	pCal(str,result);
	printf("计算结果为：%s",result);
	getchar();
	getchar();
	return 0;
}