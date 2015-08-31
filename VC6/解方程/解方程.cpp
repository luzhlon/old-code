// 解方程.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

#define ChangShu	0			//已知的数
#define WeiZhi		1			//未知数
#define KuoHao		2			//带未知数的表达式或函数
#define ExprsKuoH	3			//带括号的未知表达式

void OutInfo(char *szInfo)
{
	printf("%s\n",szInfo);
}

void InInfo(char *szOutInfo,char *pszInInfo)
{
	printf("%s\n",szOutInfo);
	scanf("%s",pszInInfo);
}

bool IsNumber(char c)
{
	if((c>='0')&&(c<='9'))
		return 1;
	return 0;
}

bool IsSign(char c)//判断正负号
{
	if(c=='-')return 1;
	if(c=='+')return 1;
	return 0;
}

bool GetAnItemStr(char *szExprs,char *pszOut)				//获取一个项的字符串
{
	int i=0;
	int k=0;//平衡括号使用
	char szBuf[1024];

	if(!szExprs[0])
		return 0;
	pszOut[i]=szExprs[i];i++;
	while(1)
	{
		if((IsSign(szExprs[i]))&&(k==0))
			break;
		if(szExprs[i]==0)
			break;
		pszOut[i]=szExprs[i];
		if(szExprs[i]=='(')
			k++;
		else if(szExprs[i]==')')
			k--;
		i++;
	}

	pszOut[i]=0;

	strcpy(szBuf,&szExprs[i]);
	strcpy(szExprs,szBuf);
	return 1;
}

bool GetAnNumStr(char *szExprs,char *pszOut)					//获取一个操作数的字符串
{
	int i=0;
	int k=0;//平衡括号使用
	char szBuf[1024];

	if(!szExprs[0])
		return 0;
	while(1)
	{
		if(((szExprs[i]=='*')||(szExprs[i]=='/'))&&(k==0))
			break;
		if(szExprs[i]==0)
			break;
		pszOut[i]=szExprs[i];
		if(szExprs[i]=='(')
			k++;
		else if(szExprs[i]==')')
			k--;
		i++;
	}

	pszOut[i]=0;

	strcpy(szBuf,&szExprs[i]);
	strcpy(szExprs,szBuf);
	return 1;
}

int GetNum(char *szItem,char *szOut,double *result)				//获得一个操作数
{
}

char GetCaoZuoFu(char *szItem)
{
	char c=szItem[0];
	int i;
	for(i=0;szItem[i]!=0;i++)
		szItem[i]=szItem[i+1];
	return c;
}

int GetItem(char *szItem,char *szOut,double *result)			//获得一个项
{
	char szBuf[1024]={0};
	char szBufs[256];
	char szWork[1024]={0};
	char szKuoHao[1024]={0};
	char szLeft[256];
	char szRight[256];
	char c;
	int i=0,type;
	double left,right;
	double fBuf,fConst=1;
	strcpy(szWork,szItem);

	if(szWork[0]=='-')fConst=-1;
	szOut[0]=0;
	while(GetAnNumStr(szWork,szLeft))
	{
		c=GetCaoZuoFu(szWork);
		if(!c)break;
		GetAnNumStr(szWork,szRight)
		type=GetNum(szLeft,szBuf,&fBuf);
		switch(type)
		{
		case ChangShu:
			if(c=='*')
				fConst*=fBuf;
			else
				fConst/=fBuf;
			break;
		case WeiZhi:
		case KuoHao:
			while(GetAnItemStr(szBuf,szBufs))
			break;
		}
			strcat(szOut,szBuf);
	}
}

int Simplify(char *szExprs,char *szOut,double *result)			//化简一个表达式
{
	int type;
	char szBuf[1024]={0};
	char szWork[1024]={0};
	double fBuf,fConst=0;
	strcpy(szWork,szExprs);//将表达式拷贝至工作缓冲区

	szOut[0]=0;
	while(GetAnItemStr(szWork,szBuf))//取出一项
	{
		type=GetItem(szBuf,szBuf,&fBuf);
		if(type==ChangShu)
			fConst+=fBuf;
		else
			strcat(szOut,szBuf);
	}

	if(szOut[0])
	{
		if(fConst>0)
		{
			sprintf(szBuf,"+%f",fConst);
		}
		else if(fConst<0)
		{
			sprintf(szBuf,"%f",fConst);
		}
		else
			return WeiZhi;
		strcat(szOut,szBuf);
		return WeiZhi;
	}
	else
	{
		sprintf(szBuf,"%f",fConst);
		return ChangShu;
	}
}

int JieFangCheng(char *szEquation)//解方程
{
	char *p=szEquation;
	char szLeft[3*1024]={0};
	char szRight[3*1024]={0};
	int i;

	for(i=0;p[i]!='=';i++)
		szLeft[i]=p[i];
	szLeft[i]=0;
	strcpy(szRight,&p[++i]);
	return 0;
}

int main(int argc, char* argv[])
{
	return 0;
}
