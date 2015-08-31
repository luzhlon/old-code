// 算式求值dll.cpp : Defines the entry point for the DLL application.
//
#include "stdafx.h"
#include "windows.h"
#include "stdlib.h"


//函数功能：计算运算符两边的数，并用结果文本替换掉算式
void CalRep(char *s)
{
	char * p;//可移动的指针
	char * pl,//指向算式字符串的左边界
		 * pr;//指向算式字符串的右边界
	int result;//存放结果
	char szBuf[512];
	int left,right;//存放运算符两边的数
	//取左边的数值
	p=s-1;
a:	if(((*p)==' ')||((*p)=='\t'))//忽略空格与制表符
	{
		p--;
		goto a;
	}
	while(1)
	{
		if(((*p)>='0')&&((*p)<='9'))	//如果字符在'0','9'之间
		{
			p--;
		}
		else
			break;
	}
	left=atoi(++p);
	//取左边界
	pl=p;
	//取右边的数值
	right=atoi(s+1);
	//取右边界
	p=s+1;
b:	if(((*p)==' ')||((*p)=='\t'))//忽略空格与制表符
	{
		p++;
		goto b;
	}
	while(1)
	{
		if(((*p)>='0')&&((*p)<='9'))	//如果字符在'0','9'之间
		{
			p++;
		}
		else
			break;
	}
	pr=--p;
	switch(*s)
	{
	case '+':
		result=left+right;
		break;
	case '-':
		result=left-right;
		break;
	case '*':
		result=left*right;
		break;
	case '/':
		result=left/right;
		break;
	}
	wsprintf(szBuf,"%d",result);
	lstrcpy(pl,szBuf);
	lstrcpy(szBuf,pr+1);//保存下算式右边的字符串
	lstrcat(pl,szBuf);
}

__declspec(dllexport) BOOL CALLBACK calculate(char *equation,char *result)
{
	char str[1024];//存放字符串
	char level[4]={'*','/','+','-'};//定义运算级
	int i;//运算符计数
	int j;//字符串计数

	lstrcpy(str,equation);
	for(i=0;i<sizeof(level);i++)
	{
		for(j=0;str[j]!=0;)
		{
			if(str[j]==level[i])
			{
				CalRep(&str[j]);
				continue;
			}
			j++;
		}
	}
	lstrcpy(result,str);
	return 1;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}
