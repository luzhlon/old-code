// keyj3.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "windows.h"


#ifndef __WINDOWS_H__
#include "windows.h"
#endif

class MKeyBoard
{
public:
	MKeyBoard();
	void up(char);
	void down(char);
	void click(char);
private:
	HANDLE hDevice;
};


//安装驱动  参数1：服务名称   参数二：驱动文件路径
bool InstallService(char *szDriver,char *szFile,bool bStart)
{
   SC_HANDLE sh=OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);
   if(!sh)return false;
   SC_HANDLE rh=CreateService(sh,szDriver,szDriver,SERVICE_ALL_ACCESS,
                                                   SERVICE_KERNEL_DRIVER,
                                                   SERVICE_DEMAND_START,
                                                   SERVICE_ERROR_NORMAL,
                                                   szFile,NULL,NULL,NULL,
                                                   NULL,NULL);
   if(!rh)
   {
     if(GetLastError()==ERROR_SERVICE_EXISTS)
     {
        //服务已安装的情况
        rh=OpenService(sh,szDriver,SERVICE_ALL_ACCESS);
        if(!rh)
        {
          CloseServiceHandle(sh);
          return false;
        }
     }
     else
     {
        CloseServiceHandle(sh);
        return false;
     }
   }
   if(bStart)                //是否要启动服务
     StartService(rh,0,NULL);
   CloseServiceHandle(sh);
   CloseServiceHandle(rh);
   return true;
}

MKeyBoard::MKeyBoard()
{
	char syspath[] = "E:\\Program Files\\Borland\\CBuilder6\\Projects\\读写键盘端口\\Keyboard.sys";
	if(!InstallService("mykey", syspath, 1))
	{
		MessageBox(0, "Error", "Install Service failue!", MB_OK);
		ExitProcess(0);
	}

	hDevice = CreateFile("\\\\.\\wriKeyPort", GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
	if(hDevice == INVALID_HANDLE_VALUE)
	{
		MessageBox(0, "Error", "Open device failue!", MB_OK);
		ExitProcess(0);
	}
}
//参数：扫描码
void MKeyBoard::up(char c)
{
	c &= 0x7f;
	DWORD n;
	WriteFile(hDevice, &c, 1, &n, 0);
	return;
}
//参数：扫描码
void MKeyBoard::down(char c)
{
	c &= 0x80;
	DWORD n;
	WriteFile(hDevice, &c, 1, &n, 0);
	return;
}
//参数：扫描码
void MKeyBoard::click(char c)
{
	down(c);
	Sleep(200);
	up(c);
	return;
}
/*
int (_stdcall *click)(int);
int (_stdcall *up)(int);
int (_stdcall *down)(int);
int (_stdcall *to)(int);
*/
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	/*
	click = (int (_stdcall *)(int))GetProcAddress(LoadLibrary("D:\\Documents\\Lua\\jx3Key.dll"),"click");
	down = (int (_stdcall *)(int))GetProcAddress(LoadLibrary("D:\\Documents\\Lua\\jx3Key.dll"),"down");
	up = (int (_stdcall *)(int))GetProcAddress(LoadLibrary("D:\\Documents\\Lua\\jx3Key.dll"),"up");
	to = (int (_stdcall *)(int))(GetProcAddress(LoadLibrary("D:\\Documents\\Lua\\jx3Key.dll"),"toScanCode"));
	
	while(1)
	{
		click((int)MapVirtualKey((UINT)'3', 0));
		Sleep(500);
	}*/
	//*
 	// TODO: Place code here.
	MKeyBoard k;
	char c = MapVirtualKey((UINT)'3', 0);
	Sleep(2000);
	k.click(c);

	return 0;
}



