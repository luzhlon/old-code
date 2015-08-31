//BY CodeSoul
#include "bs64.h"

//base64转码函数

//编码函数
//参数c：要编码的缓冲区
//参数b：编码后的缓冲区
//参数len：编码长度
//返回编码后的缓冲区长度
int ToB( char *c, char *b ,int len)
{
	int q ;//商
	int r ;//余数
	char pctemp[4] = {0};
	int *p = (int *)pctemp;
	char base64_table[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	q = len / 3;
	r = len % 3;

	for(; q ; q-- , c+=3 ,b+=4 )
	{
		pctemp[2] = c[0];
		pctemp[1] = c[1];
		pctemp[0] = c[2];
		b[3] = base64_table[ (*p) & 0x3F ];
		b[2] = base64_table[ ((*p) >> 6) & 0x3F ];
		b[1] = base64_table[ ((*p) >> 12) & 0x3F ];
		b[0] = base64_table[ ((*p) >> 18) & 0x3F ];
	}

	switch( r )
	{
	case 1:
		b[0] = base64_table[ ((*c) >> 2) & 0x3F ];
		b[1] = base64_table[ ((*c) & 3) << 4 ];
		b[2] = b[3] = '=';
		break;
	case 2:
		pctemp[2] = 0;
		pctemp[1] = c[0];
		pctemp[0] = c[1];
		b[0] = base64_table[ ((*p) >> 10) & 0x3F ];
		b[1] = base64_table[ ((*p) >> 4) & 0x3F ];
		b[2] = base64_table[ ((*p) & 15) << 2 ];
		b[3] = '=';
		break;
	}

	return r ? 4*(1+q) : 4*q;
}

//解码函数
//参数b：要解码的缓冲区
//参数c：解码后的缓冲区
//参数len：解码长度
//返回解码后的缓冲区长度

int ToC(char* b, char* c, int len)
{
    int nDstLen = 0;            // 输出的字符计数
    int i;
	char pctemp[4] = {0};
	int *p = (int *)pctemp;
	int m,n;//中间数，存放数值用

	char DeBase64Tab[] =
	{
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	   62,        // '+'
	   0, 0, 0,
	  63,        // '/'
	   52, 53, 54, 55, 56, 57, 58, 59, 60, 61,        // '0'-'9'
	    0, 0, 0, 0, 0, 0, 0,
	   0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
	   13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,        // 'A'-'Z'
	    0, 0, 0, 0, 0, 0,
	    26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
	   39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,        // 'a'-'z'
	};

	for( n=0; len>0; n=0  )
	{
		
		for( i = 0; i<4; b++,len-- )//取4个字符，不包括换行符
		{
			if( *b == '\r' || *b == '\n' )//忽略换行符
			{
				continue;
			}
			pctemp[i] = *b;
			i++;
		}

		if( pctemp[3] == '=' )
		{
			if( pctemp[2] == '=' )
			{
				*c = 0;
				*c |= DeBase64Tab[ pctemp[0] ] << 2;
				*c |= DeBase64Tab[ pctemp[1] ] >> 4;
				nDstLen++;
				break;
			}

			m = (int)DeBase64Tab[ pctemp[0] ];
			n |= m << 10;
			m = (int)DeBase64Tab[ pctemp[1] ];
			n |= m << 4;
			m = (int)DeBase64Tab[ pctemp[2] ];
			n |= m >> 2;
			c[0] = pctemp[1];
			c[1] = pctemp[0];
			nDstLen += 2;
			break;
		}

		
		m = (int)DeBase64Tab[ pctemp[0] ];
		n |= m << 18;
		m = (int)DeBase64Tab[ pctemp[1] ];
		n |= m << 12;
		m = (int)DeBase64Tab[ pctemp[2] ];
		n |= m << 6;
		m = (int)DeBase64Tab[ pctemp[3] ];
		n |= m;
		*p = n;
		c[0] = pctemp[2];
		c[1] = pctemp[1];
		c[2] = pctemp[0];
		c += 3;
		nDstLen += 3;
	}
 
	return nDstLen;
}




/*
0	 A	 16	 Q	 32	 g	 48	 w
1	 B	 17	 R	 33	 h	 49	 x
2	 C	 18	 S	 34	 i	 50	 y
3	 D	 19	 T	 35	 j	 51	 z
4	 E	 20	 U	 36	 k	 52	 0
5	 F	 21	 V	 37	 l	 53	 1
6	 G	 22	 W	 38	 m	 54	 2
7	 H	 23	 X	 39	 n	 55	 3
8	 I	 24	 Y	 40	 o	 56	 4
9	 J	 25	 Z	 41	 p	 57	 5
10	 K	 26	 a	 42	 q	 58	 6
11	 L	 27	 b	 43	 r	 59	 7
12	 M	 28	 c	 44	 s	 60	 8
13	 N	 29	 d	 45	 t	 61	 9
14	 O	 30	 e	 46	 u	 62	 +
15	 P	 31	 f	 47	 v	 63	 /
*/
