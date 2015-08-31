// gethostbyname.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "Winsock2.h"

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
	char   ptr[256]={0};
    hostent * hptr;
    char   str[32];
	int i=0;
	in_addr a;

	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData))
		MessageBox(NULL,"初始化失败","Message",MB_OK);

	printf("请输入域名:");
	scanf("%s",ptr);

    if((hptr=gethostbyname(ptr))==NULL)
    {
        printf(" gethostbyname error for host:%s\n", ptr);
		getchar();
        return 0;
    }

	for(;hptr->h_addr_list[i];i++)
	{
		memcpy(&a,hptr->h_addr_list[i],4);
		printf("地址之一：%s\n",inet_ntoa(a));
	}

	getchar();
	getchar();
    return 0;
}

