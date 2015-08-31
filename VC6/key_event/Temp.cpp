// Temp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include <windows.h>

int main(int argc, char* argv[])
{
	while( 1 )
	{
	 keybd_event( 0x4A,
                      0,
                      0,
                      0 );

	 keybd_event( 0x4A,
                      0,
                      KEYEVENTF_KEYUP,
                      0);

	 Sleep( 150 );
	}
	getchar();
	return 0;
}
