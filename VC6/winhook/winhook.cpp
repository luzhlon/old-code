// winhook.cpp : Defines the entry point for the DLL application.
//此模块实现三个功能：安装钩子，卸载钩子，检测钩子提前卸载钩子

#include <windows.h>
#include "stdafx.h"
#include "winhook.h"

#pragma comment(linker, "/SECTION:Shared,RWS")		//共享dll数据段
HWND hShell = 0;

//钩子回调函数
int HOOKPROC HookProc( int dwCode, int wParam, int lParam )
{
	return 0;		//拦截此消息，阻止其被发送到目标窗口
}

//安装键盘鼠标钩子
WINHOOK_API int InstallHook()
{
	return 1;
}

//卸载键盘鼠标钩子
WINHOOK_API int UninstallHook()
{
	return 1;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}


// This is an example of an exported variable
WINHOOK_API int nWinhook=0;

// This is an example of an exported function.
WINHOOK_API int fnWinhook(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see winhook.h for the class definition
CWinhook::CWinhook()
{ 
	return; 
}

