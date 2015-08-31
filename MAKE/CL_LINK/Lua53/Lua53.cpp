#include "lua.hpp"
#include <stdio.h>
#include <string.h>

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
