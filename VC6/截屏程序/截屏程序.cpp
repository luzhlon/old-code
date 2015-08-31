// 截屏程序.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include <windows.h>

HINSTANCE	hInst;

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
     static TCHAR szAppName[] = TEXT ("截屏") ;
     HWND         hwnd ;
     MSG          msg ;
     WNDCLASS     wndclass ;
	 hInst=hInstance;

     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;

     if (!RegisterClass (&wndclass))
     {
          MessageBox (NULL, TEXT ("This program requires Windows NT!"), 
                      szAppName, MB_ICONERROR) ;
          return 0 ;
     }
     
     hwnd = CreateWindow (szAppName,                  // window class name
                          TEXT ("The Hello Program"), // window caption
                          WS_OVERLAPPEDWINDOW
						  &(~(WS_THICKFRAME|WS_SIZEBOX)) ,        // window style
                          CW_USEDEFAULT,              // initial x position
                          CW_USEDEFAULT,              // initial y position
                          CW_USEDEFAULT,              // initial x size
                          CW_USEDEFAULT,              // initial y size
                          NULL,                       // parent window handle
                          NULL,                       // window menu handle
                          hInstance,                  // program instance handle
                          NULL) ;                     // creation parameters
     
     ShowWindow (hwnd, iCmdShow) ;
     UpdateWindow (hwnd) ;
     
     while (GetMessage (&msg, NULL, 0, 0))
     {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
     }
     return msg.wParam ;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     HDC         hdc,hdc1;
	 static	HDC	hCdc;
	 static HANDLE hBitMap;
     PAINTSTRUCT ps ;
     static RECT rect ;
     
     switch (message)
     {
     case WM_CREATE:
          CreateWindow("button","截屏",WS_CHILD|WS_VISIBLE,0,0,100,50,hwnd,(struct HMENU__ *)1,hInst,NULL);
		  hdc=GetDC(hwnd);
		  GetClientRect (hwnd, &rect) ;
		  hBitMap=CreateCompatibleBitmap(hdc,rect.right-rect.left,rect.bottom-rect.top);
		  hCdc=CreateCompatibleDC(hdc);
		  SelectObject(hCdc,hBitMap);
		  ReleaseDC(hwnd,hdc);
          return 0 ;
          
     case WM_PAINT:
          hdc = BeginPaint (hwnd, &ps) ;          
          BitBlt(hdc,0,0,rect.right-rect.left,rect.bottom-rect.top,hCdc,0,0,SRCCOPY);
		  InvalidateRect(hwnd,&rect,0);
          EndPaint (hwnd, &ps) ;
          return 0 ;
          
	 case WM_COMMAND:
		 switch(LOWORD(wParam))					//处理不同的子窗口
		 {
		 case 1:
			 if(HIWORD(wParam)==BN_CLICKED)					//子窗口事件
			 {
				 hdc1=GetDC(hwnd);
				 hdc=GetWindowDC(NULL);
				 BitBlt(hCdc,0,0,rect.right-rect.left,rect.bottom-rect.top,hdc,0,0,SRCCOPY);
				 ReleaseDC(hwnd,hdc1);
			 }
		 }
		  return 0;
     case WM_DESTROY:
          PostQuitMessage (0) ;
          return 0 ;
     }
     return DefWindowProc (hwnd, message, wParam, lParam) ;
}


