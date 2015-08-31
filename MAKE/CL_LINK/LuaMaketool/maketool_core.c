#include <windows.h>
#include "lua.h"
#include "lauxlib.h"

static int if_new(lua_State *L)
{
    const char *f1 = luaL_checkstring(L, 1); 
    const char *f2 = luaL_checkstring(L, 2); 
    WIN32_FILE_ATTRIBUTE_DATA fa1 = {0},
                              fa2 = {0};
    int bNew = 0;

    if(!GetFileAttributesEx(f1, GetFileExInfoStandard, &fa1))
    {
        luaL_error(L, "Get file1 date&time failue!");
        return 0;
    }

    if(!GetFileAttributesEx(f2, GetFileExInfoStandard, &fa2))
    {
        luaL_error(L, "Get file2 date&time failue!");
        return 0;
    }

    if(fa1.ftLastWriteTime.dwHighDateTime > fa2.ftLastWriteTime.dwHighDateTime)
        bNew = 1;
    else
    if(fa1.ftLastWriteTime.dwLowDateTime > fa2.ftLastWriteTime.dwLowDateTime)
        bNew = 1;

	lua_pushboolean(L, bNew);

	return 1;
}

static const struct luaL_reg maketool[] = 
{
	{"if_new", if_new},
	{0, 0}
};

int __declspec(dllexport) luaopen_maketool_core(lua_State *L)
{
	char szLibName[] = "maketool";
	luaL_openlib(L, szLibName, maketool, 0);
	return 1;
}


int _stdcall DllMain(HINSTANCE hInst, DWORD dwReason, DWORD dwReserved)
{
	return 1;
}


