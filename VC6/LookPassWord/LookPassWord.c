// LookPassWord.cpp : Defines the entry point for the application.
//

#include "resource.h"
#include <windows.h>

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{     
	POINT pt;
	HWND hWnd;
	char szCap[1024];
     switch (message)
     {
     case WM_CLOSE:
		  KillTimer(hwnd,0);
          EndDialog(hwnd,0);
          return 1;
          
     case WM_INITDIALOG:
          SetWindowPos(hwnd,HWND_TOPMOST,0,0,400,300,SWP_SHOWWINDOW);
		  SetTimer(hwnd,0,100,NULL);
		  return 1;
     case WM_TIMER:
          GetCursorPos(&pt);
		  hWnd=WindowFromPoint(pt);
		  GetWindowText(hWnd,szCap,1024);
		  SetDlgItemText(hwnd,IDC_SHOW,szCap);
          return 1 ;
     }
     return 0;
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG1),NULL,(DLGPROC)WndProc,0);
	return 0;
}



