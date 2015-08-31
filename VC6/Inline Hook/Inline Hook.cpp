// SimpleHook.cpp : Defines the entry point for the DLL application.
//

#include "windows.h"
#include "stdafx.h"
DWORD* lpAddr;
PROC OldProc;
BOOL __stdcall MyTerminateProcess(HANDLE hProcess,UINT uExitCode)
{
	MessageBox(NULL,"没法结束进程了吧 ","API HOOK",0);
	return 0;
}


int ApiHook(char *DllName,//DLL文件名
     char *ProcName,//要HOOK的函数名称
     PROC NewFunAddr//我们够造的函数地址
     )
{
	HMODULE hMoudle=LoadLibrary(DllName);
	HWND hParent;
	OldProc=GetProcAddress(hMoudle,ProcName);
	//计算原函数地址与我们的函数的偏移
	DWORD offsets=(int)MyTerminateProcess-(int)OldProc-5;
	//写入原函数入口处jmp MyTerminateProcess指令
	//由于不能修改本进程指令，这一段注视掉，改为向父进程发送数据，让父进程实现
/*	_asm
	{
		push ebx
		mov ebx,OldProc
		mov byte ptr [ebx],0xE9
		inc ebx
		push offsets
		pop [ebx]
		pop ebx
	}  */
	//查找父进程窗口句柄
	hParent=FindWindow(NULL,"Inline Hook");
	if(!hParent)
		MessageBox(NULL,"Not find the parent window","Error",MB_OK);
	//自定义消息的参数1为要Hook的入口地址，参数2为要跳转的偏移地址
	Sleep(5000);
	SendMessage(hParent,WM_USER+1,(unsigned int)OldProc,(unsigned int)offsets);
	return 0;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
       DWORD ul_reason_for_call, 
       LPVOID lpReserved
       )
{	if(ul_reason_for_call==DLL_PROCESS_ATTACH)
		ApiHook("ntdll.dll","ZwTerminateProcess",(PROC)MyTerminateProcess);
    return TRUE; 
}