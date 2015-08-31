// SimpleHook.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
DWORD* lpAddr;
PROC OldProc;
BOOL __stdcall MyTerminateProcess(HANDLE hProcess,UINT uExitCode)
{
MessageBox(NULL,"没法结束进程了吧 ","API HOOK",0);
return 0;
}

int ApiHook(char *DllName,//DLL文件名
     PROC OldFunAddr,//要HOOK的函数地址
     PROC NewFunAddr//我们够造的函数地址
     )
{
//得到函数进程模块基地址
HMODULE lpBase = GetModuleHandle(NULL); //返回当前exe的基地址
IMAGE_DOS_HEADER *dosHeader; //定义dos头结构
IMAGE_NT_HEADERS *ntHeader; //PE结构
IMAGE_IMPORT_BY_NAME *ImportName;//输入表中 函数名字和序号存放的结构
//定位到DOS头
dosHeader=(IMAGE_DOS_HEADER*)lpBase; //获得当前进程基地址和dos头
//定位到PE头
ntHeader=(IMAGE_NT_HEADERS32*)((BYTE*)lpBase+dosHeader->e_lfanew);// e_lfanew存放的是pe头的偏移~转换结构类型
//检查pe文件是否有效
if(ntHeader->Signature !=IMAGE_NT_SIGNATURE)MessageBox(NULL,"不是有效的pe文件","错误",MB_OK);
//定位到导入表
IMAGE_IMPORT_DESCRIPTOR *pImportDesc=(IMAGE_IMPORT_DESCRIPTOR*)((BYTE*)lpBase+ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
//基地址+输入表的起始RVA

// 就是 得到 iat结构的RVA,如果这个dll文件不是要hook的函数,的那个DLL,则++到下个dll的PE中
while(pImportDesc->FirstThunk)
{
   //得到DLL文件名
   char* pszDllName = (char*)((BYTE*)lpBase + pImportDesc->Name);
   //比较得到的DLL文件名是否和要HOOK函数所在的DLL相同
   if(lstrcmpiA(pszDllName, DllName) == 0)
   {
    break;
   }
   pImportDesc++;
}
//定位到FirstThunk参数指向的IMAGE_THUNK_DATA，此时这个结构已经是函数入口点地址了
IMAGE_THUNK_DATA* pThunk = (IMAGE_THUNK_DATA*)
   ((BYTE*)lpBase + pImportDesc->FirstThunk); //得到这个要hook的函数的dll的IAT
//遍历这部分IAT表
while(pThunk->u1.Function)
{
   lpAddr = (DWORD*)&(pThunk->u1.Function);
   //比较函数地址是否相同
   if(*lpAddr == (DWORD)OldFunAddr)
   { 
    DWORD dwOldProtect;
    //修改内存包含属性
    VirtualProtect(lpAddr, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect);
    //API函数的入口点地址改成我们构造的函数的地址
    WriteProcessMemory(GetCurrentProcess(),lpAddr, &NewFunAddr, sizeof(DWORD), NULL);
   }
   pThunk++;
}
return 0;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
       DWORD ul_reason_for_call, 
       LPVOID lpReserved
       )
{
switch (ul_reason_for_call)
{
case DLL_PROCESS_ATTACH:
   //得到TerminateProcess函数地址
   OldProc = (PROC)TerminateProcess;
   //定位，修改IAT表
   ApiHook("kernel32.dll",OldProc,(PROC)MyTerminateProcess);
   break;
case DLL_PROCESS_DETACH:
   //恢复IAT表中API函数的入口点地址
   WriteProcessMemory(GetCurrentProcess(),lpAddr, &OldProc, sizeof(DWORD), NULL);
   break; 
}
    return TRUE; 
}