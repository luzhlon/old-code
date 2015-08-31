// ´úÂëGhost.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "windows.h"

int c4=MB_OK;
char *s1="CodeSoul";
char *s2="Hello world!";
int c1=NULL;
int pr=(int)MessageBox;
int str[5];

void Init();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	int i;
	int p;
	Init();
	for(i=0;i<=4;i++)
	{
		p=(int)str[i];
		if(i!=4)
		{
		_asm push p
		}
		else
		{
		_asm call p
		}
	}
	return 0;
}

void Init()
{
	str[0]=c4;str[1]=(int)s1;str[2]=(int)s2;str[3]=c1;str[4]=pr;
}



