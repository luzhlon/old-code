#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>
#include <dbgeng.h>
#include "lua.hpp"

#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "dbgeng.lib")

char filed_tar_handle[] = "tar_handle";
char filed_tar_id[] = "tar_id";
char filed_lastProcessID[] = "lastProcessID";
char filed_lastThreadID[] = "lastThreadID";

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

struct luaL_reg LuaDebugLib_core[] = 
{
	{"createTarget", createTarget},
	{"attachTarget", attachTarget},
	{"readMem", readMem},
	{"writeMem", writeMem},
	{"allocMem", allocMem},
	{"continueDebug", continueDebug},
	{"getContext", getContext},
	{"setContext", setContext},
	{"waitEvent", waitEvent},
	/*
	{},
	{},
	{},
	{},
	{},
	{},*/
	{0, 0}
};

extern "C" int __declspec(dllexport) luaopen_LuaDebugLib_core(lua_State *L)
{
	char szLibName[] = "LuaDebugLib_core";
	luaL_openlib(L, szLibName, LuaDebugLib_core, 0);
	//MessageBox(0, "aaaaaaa", "aaaaaa", 0);
	return 1;
}

int main()
{
    char buf[1024];
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    while(fgets(buf, sizeof(buf), stdin))
    {
        int err = luaL_loadbuffer(L, buf, strlen(buf), "line") || lua_pcall(L, 0, 0, 0);
        if(err)
        {
            fprintf(stderr, "%s", lua_tostring(L, -1));
            lua_pop(L, 1);
        }
    }
    lua_close(L);
    return 0;
}
