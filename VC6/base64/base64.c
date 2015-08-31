// base64.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <string.h>
#include "bs64.h"

int main(int argc, char* argv[])
{
	char buf[100*1024] = {0};
	char buf2[100*1024] = {0};
	int  len,i;

	FILE *a = fopen( "pre.txt", "r" ); 
	for( i = 0; !feof( a ); i++ )
	{
		buf[i] = fgetc( a );
	}

	printf( buf );

	printf( "\n%d\n", i );

	len = ToC( buf, buf2, strlen(buf) );

	FILE *f = fopen( "file.txt", "wb" );
	fwrite( buf2, len, 1, f );
	printf( buf2 );
	getchar();
	return 0;
}
