#include "LuaAPI.h"

LuaAPI::LuaAPI(lua_State *L)
{
	if (L)
	{
		m_L = L;
		return;
	}
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
}


LuaAPI::~LuaAPI()
{
	lua_close(m_L);
}
