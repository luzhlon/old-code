#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <dbgeng.h>

IDebugClient* g_Client;
IDebugControl* g_Control;
IDebugDataSpaces* g_Data;
IDebugRegisters* g_Registers;
IDebugSymbols* g_Symbols;

struct BREAKPOINT
{
    IDebugBreakpoint* Bp;
    ULONG Id;
};

BREAKPOINT g_GetVersionBp;
BREAKPOINT g_GetVersionRetBp;
BREAKPOINT g_GetVersionExBp;
BREAKPOINT g_GetVersionExRetBp;

ULONG g_EaxIndex = DEBUG_ANY_ID;
OSVERSIONINFO g_OsVer;
DWORD g_VersionNumber;
ULONG64 g_OsVerOffset;

void Exit(int Code, PCSTR Format, ...)
{
    // Clean up any resources.
    if (g_Control != NULL)
    {
        g_Control->Release();
    }
    if (g_Data != NULL)
    {
        g_Data->Release();
    }
    if (g_Registers != NULL)
    {
        g_Registers->Release();
    }
    if (g_Symbols != NULL)
    {
        g_Symbols->Release();
    }
    if (g_Client != NULL)
    {
        //
        // Request a simple end to any current session.
        // This may or may not do anything but it isn't
        // harmful to call it.
        //

        g_Client->EndSession(DEBUG_END_PASSIVE);
        
        g_Client->Release();
    }

    // Output an error message if given.
    if (Format != NULL)
    {
        va_list Args;

        va_start(Args, Format);
        vfprintf(stderr, Format, Args);
        va_end(Args);
    }
    
    exit(Code);
}

void CreateInterfaces(void)
{
	SYSTEM_INFO SysInfo;

	// For purposes of keeping this example simple the
	// code only works on x86 machines.  There's no reason
	// that it couldn't be made to work on all processors, though.
	GetSystemInfo(&SysInfo);
	if (SysInfo.wProcessorArchitecture != PROCESSOR_ARCHITECTURE_INTEL)
	{
		Exit(1, "This program only runs on x86 machines.\n");
	}

	// Get default version information.
	g_OsVer.dwOSVersionInfoSize = sizeof(g_OsVer);
	if (!GetVersionEx(&g_OsVer))
	{
		Exit(1, "GetVersionEx failed, %d\n", GetLastError());
	}

	HRESULT Status;

	// Start things off by getting an initial interface from
	// the engine.  This can be any engine interface but is
	// generally IDebugClient as the client interface is
	// where sessions are started.
	if ((Status = DebugCreate(__uuidof(IDebugClient),
		(void**)&g_Client)) != S_OK)
	{
		Exit(1, "DebugCreate failed, 0x%X\n", Status);
	}

	// Query for some other interfaces that we'll need.
	if ((Status = g_Client->QueryInterface(__uuidof(IDebugControl),
		(void**)&g_Control)) != S_OK ||
		(Status = g_Client->QueryInterface(__uuidof(IDebugDataSpaces),
		(void**)&g_Data)) != S_OK ||
		(Status = g_Client->QueryInterface(__uuidof(IDebugRegisters),
		(void**)&g_Registers)) != S_OK ||
		(Status = g_Client->QueryInterface(__uuidof(IDebugSymbols),
		(void**)&g_Symbols)) != S_OK)
	{
		Exit(1, "QueryInterface failed, 0x%X\n", Status);
	}
}

class InputCallBack: public IDebugInputCallbacks
{
public:
    virtual ULONG _stdcall AddRef()
    {
        return 0;
    }
    virtual ULONG _stdcall Release()
    {
        return 1;
    }
    virtual HRESULT _stdcall QueryInterface(REFIID id, void **ppvObj)
    {
        *ppvObj = this;
        return NOERROR;
    }

    virtual HRESULT _stdcall StartInput(IN ULONG BuffSize)
    {
        printf("[StartInput] BuffSize:%u\n", BuffSize);
        return 0;
    }

    virtual HRESULT _stdcall EndInput()
    {
        printf("[EndInput]\n");
        return 0;
    }
};

class OutputCallBack: public IDebugOutputCallbacks
{
public:
    virtual ULONG _stdcall AddRef()
    {
        return 0;
    }
    virtual ULONG _stdcall Release()
    {
        return 1;
    }
    virtual HRESULT _stdcall QueryInterface(REFIID id, void **ppvObj)
    {
        *ppvObj = NULL;

        if (IsEqualIID(id, __uuidof(IUnknown)) ||
            IsEqualIID(id, __uuidof(IDebugOutputCallbacks)))
        {
            *ppvObj = (IDebugOutputCallbacks *)this;
            AddRef();
            return S_OK;
        }
        else
        {
            return E_NOINTERFACE;
        }
    }

    virtual HRESULT _stdcall Output(IN ULONG Mask, IN PCSTR Text)
    {
        int len = strlen(Text);
        char *out = (char *)malloc(len+128);
        char typ[128] = " ";

        if(Mask & DEBUG_OUTPUT_NORMAL)
            strcat(typ, "NORMAL ");
        else if(Mask & DEBUG_OUTPUT_ERROR)
            strcat(typ, "ERROR ");
        else if(Mask & DEBUG_OUTPUT_WARNING)
            strcat(typ, "WARNING ");
        else if(Mask & DEBUG_OUTPUT_VERBOSE)
            strcat(typ, "VERBOSE ");
        else if(Mask & DEBUG_OUTPUT_PROMPT)
            strcat(typ, "PROMPT ");
        else if(Mask & DEBUG_OUTPUT_PROMPT_REGISTERS)
            strcat(typ, "PROMPT_REGISTERS ");
        else if(Mask & DEBUG_OUTPUT_DEBUGGEE)
            strcat(typ, "DEBUGGEE ");
        else if(Mask & DEBUG_OUTPUT_SYMBOLS)
            strcat(typ, "SYMBOLS ");
        else if(Mask & DEBUG_OUTPUT_EXTENSION_WARNING)
            strcat(typ, "EXTENSION_WARNING ");
        else if(Mask & DEBUG_OUTPUT_DEBUGGEE_PROMPT)
            strcat(typ, "DEBUGGEE_PROMPT ");

        sprintf(out, "[%s] %s", typ, Text);
        puts(out);

        free(out);
        return 0;
    }
};

class EventCallBack: public IDebugEventCallbacks
{
public:
    // IUnknown.
    virtual ULONG _stdcall AddRef()
    {
        return 0;
    }
    virtual ULONG _stdcall Release()
    {
        return 1;
    }
    virtual HRESULT _stdcall QueryInterface(REFIID id, void **ppvObj)
    {
        *ppvObj = this;
        return NOERROR;
    }


    virtual HRESULT _stdcall GetInterestMask(PULONG Mask)
    {
        return S_OK;
    }
    virtual HRESULT _stdcall Breakpoint(IDebugBreakpoint *bp)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall ChangeDebuggeeState(ULONG Flags, ULONG64 Argument)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall ChangeEngineState(ULONG Flags, ULONG64 Argument)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall Exception(PEXCEPTION_RECORD64 Exception, ULONG FirstChance)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall UnloadModule(PCSTR ImageBaseName, ULONG64 BaseOffset)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall ExitProcess(ULONG ExitCode)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall SessionStatus(ULONG Status)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall ChangeSymbolState(ULONG Flags, ULONG64 Argument)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall SystemError(ULONG Error, ULONG Level)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall CreateThread(ULONG64 Handle, ULONG64 DateOffset, ULONG64 StartOffset)
    {
        printf("[Event:]Thread baseAddr:0x%x\n", StartOffset);
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall ExitThread(ULONG ExitCode)
    {
        return DEBUG_STATUS_GO_HANDLED;
    }
    virtual HRESULT _stdcall LoadModule(
            IN ULONG64  ImageFileHandle,
            IN ULONG64  BaseOffset,
            IN ULONG  ModuleSize,
            IN PCSTR  ModuleName,
            IN PCSTR  ImageName,
            IN ULONG  CheckSum,
            IN ULONG  TimeDateStamp)
    {
        printf("[Event:]Load module:%s\n", ModuleName);
        return DEBUG_STATUS_GO_HANDLED;
    }


    virtual HRESULT _stdcall CreateProcess(
        IN ULONG64  ImageFileHandle,
        IN ULONG64  Handle,
        IN ULONG64  BaseOffset,
        IN ULONG  ModuleSize,
        IN PCSTR  ModuleName,
        IN PCSTR  ImageName,
        IN ULONG  CheckSum,
        IN ULONG  TimeDateStamp,
        IN ULONG64  InitialThreadHandle,
        IN ULONG64  ThreadDataOffset,
        IN ULONG64  StartOffset
        );

};

HRESULT EventCallBack::CreateProcess(
    IN ULONG64  ImageFileHandle,
    IN ULONG64  Handle,
    IN ULONG64  BaseOffset,
    IN ULONG  ModuleSize,
    IN PCSTR  ModuleName,
    IN PCSTR  ImageName,
    IN ULONG  CheckSum,
    IN ULONG  TimeDateStamp,
    IN ULONG64  InitialThreadHandle,
    IN ULONG64  ThreadDataOffset,
    IN ULONG64  StartOffset
    )
{
    printf("ImageName:%s\n", ImageName);
    return DEBUG_STATUS_GO_HANDLED;
}



//char szDebugee[256] = "E:\\Program Files (x86)\\锐捷网络\\Ruijie Supplicant\\RuijieSupplicant.exe";
///char szDebugee[256] = "D:\\BaiDuYun\\Projects\\VS201X\\lua\\Debug\\lua.exe";
char szDebugee[] = "D:\\BaiDuYun\\Projects\\VC6\\压缩\\Debug\\压缩.exe";
#define CheckResult(hr, strErr) if(hr != S_OK)\
                        {\
                            puts(strErr);\
                            return 1;\
                        }

InputCallBack input;
EventCallBack event;
OutputCallBack output;

int main(int argc, char **argv)
{
    HRESULT hr; //用于接收大多数函数的返回值
    IDebugClient *DebugClient;
    IDebugControl * DebugControl;

    //创建DebugClient
    //DebugClient 主要有两种获取方式:
    //          本地调试(Live)用DebugCreate，远程调试(Proxy)用DebugConnect
    hr = DebugCreate(__uuidof(IDebugClient), (void **)&DebugClient);
    CheckResult(hr, "[Error:]Create DebugClient failed.\n")
    hr = DebugClient->QueryInterface(__uuidof(IDebugControl), (void **)&DebugControl);
    CheckResult(hr, "[Error:]Query DebugControl failed.\n")

    hr = DebugClient->SetEventCallbacks(&event);
    CheckResult(hr, "[Error:]SetEventCallbacks failed.\n")
    printf("SetEventCallbacks success!\n");

    hr = DebugClient->SetInputCallbacks(&input);
    CheckResult(hr, "[Error:]SetInputCallbacks failed.\n")
    printf("SetInputCallbacks success!\n");
    //
    hr = DebugClient->SetOutputCallbacks(&output);
    CheckResult(hr, "[Error:]SetOutputCallbacks failed.\n")
    printf("SetOutputCallbacks success!\n");

    hr = DebugClient->CreateProcess(0, szDebugee, DEBUG_CREATE_PROCESS_NO_DEBUG_HEAP|DEBUG_PROCESS);
    //hr = DebugClient->CreateProcessAndAttach(0, szDebugee, DEBUG_CREATE_PROCESS_NO_DEBUG_HEAP, 0, DEBUG_ATTACH_EXISTING);
    CheckResult(hr, "[Error:]CreateProcess failed\n")
    /*
    ULONG pID;
    if(S_OK != DebugClient->
            GetRunningProcessSystemIdByExecutableName
            (0, "taskmgr.exe", DEBUG_GET_PROC_ONLY_MATCH, &pID))
    {
        printf("[Error:]GetID failed.\n");
        return 1;
    }
    else
    {
        printf("ID:%u\n", pID);
    }

    */
    //Event loop
    while(1)
    {
        hr = DebugControl->WaitForEvent(DEBUG_WAIT_DEFAULT,INFINITE);
        CheckResult(hr, "WaitForEvent failed.  Exiting...");
    }

    hr = DebugClient->TerminateProcesses();
    CheckResult(hr, "[Error:]TerminateProcesses failed\n")

    hr = DebugClient->EndSession(DEBUG_END_PASSIVE);
    CheckResult(hr, "[Error:]EndSession failed\n")

    DebugClient->Release();
    return 0;
}
