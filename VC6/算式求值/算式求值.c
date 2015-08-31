#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int calculate(char *equation,double *result);
int Filter(char *sr,double *rslt);

double ItemNum(char *s)
{
	char * p=s;
	double result=1;
	double left,right,sign=1;

	if(!(*s))
		return 0;

	if(*p=='+')
	{
		sign=1;
		p++;
	}
	else if(*p=='-')
	{
		sign=-1;
		p++;
	}

	if(*p=='(')
	{
		p=p+Filter(p,&left)+1;
	}
	else
		left=atof(p);

	for(;(*p)!=0;p++)
	{
		if((*p)=='*')
		{
			if(*(++p)=='(')
			{
				p=p+Filter(p,&right)+1;
			}
			else
			{
				right=atof(p);
			}
			left*=right;
			continue;
		}
		if((*p)=='/')
		{
			if(*(++p)=='(')
			{
				p=p+Filter(p,&right)+1;
			}
			else
			{
				right=atof(p);
			}
			left/=right;
			continue;
		}
	}
	//printf("%lf\n",result);
	return left*sign;
}
//计算括号里的内容并替换 
int Filter(char *sr,double *rslt)
{
	int i=0,j=0;
	int k=0;//平衡括号配对使用
	double result;
	char d[512];
	char szBuf[512];
	char *s=sr;

	i++;k++;//左括号的位置
	for(j=0;s[i]!=0;j++,i++)
	{
		if(s[i]=='(')k++;
		if(s[i]==')')k--;
		d[j]=s[i];
		if(!k)//k=0时，括号以平衡
		{
			d[j]=0;
			calculate(d,rslt);
			(*s)=&s[i];
			return j;
		}
	}
	if(k)//如果k！=0，则括号不配对
		return -1;
	return 0;
}

int GetItem(char *s,char *d)
{
	int i=0,k=0;
	d[i]=s[i];
	if(s[i]=='(')k++;
	if(s[i]==')')k--;
	i++;
	for(;s[i]!=0;i++)
	{
		if(s[i]=='(')
			k++;
		if(s[i]==')')
			k--;
		d[i]=s[i];
		if((s[i]=='+')||(s[i]=='-'))
		{
			if(k)continue;
			if((s[i-1]=='*')||(s[i-1]=='/'))continue;
			break;
		}
	}
	if(k)
		return -1;
	d[i]=0;
	return i;
}

int calculate(char *equation,double *result)
{
	char *s;
	char str[1024]={0};
	char szItem[1024]={0};
	double rslt=0;
	double dfBuf;
	int i,b;

	strcpy(str,equation);
	s=str;

	for(i=0;(*s)!=0;i++)
	{
		if(((*s)=='+')||((*s)=='-')||(i==0))
		{
			b=GetItem(s,szItem);
			if(b==-1)
				return -1;
			s=s+b;
			rslt+=ItemNum(szItem);
			continue;
		}
		s++;
	}

	(*result)=rslt;
	return 1;
}

int main()
{
	char str[1024];
	char es[256]={0};
	double result;
	int i,j;

	printf("输入算式：");
	scanf("%s",str);
	printf("计算结果：");
	for(i=0,j=0;str[i]!=0;i++,j++)
	{
		es[j]=str[i];
		if(str[i]==',')
		{
			es[j]=0;
			if(calculate(es,&result)==-1)
			{
				printf("括号不配对");
				continue;
			}
			printf("%s=%g,",es,result);
			j=0;j--;
		}
	}
	if(calculate(es,&result)==-1)
		printf("括号不配对");
	else
		printf("%s=%g",es,result);
	getchar();
	getchar();
	return 0;
}