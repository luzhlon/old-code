//因为拦截窗口时参数的结构和已知的unicodestring结构不吻合
//所以重定义一份
typedef struct winstring
{
	int length;
	int max;
	TCHAR *buf;
}WINUNICODESTRING,*PWINUNICODESTRING;

//字符串复制
int CopyWinUnicodeString(wchar_t* d,wchar_t* s,int max)
{
	int  i=0;
	while(1)
	{
		d[i]=s[i];
		if(!d[i]){break;}
		i++;
		max--;
		if(!max)
		{
			d[i]=0;
		}
	}
	return i;
}