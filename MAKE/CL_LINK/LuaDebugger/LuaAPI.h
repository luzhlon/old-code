#pragma once
#ifndef _LUAAPI_H_
#define _LUAAPI_H_

#include "lua.hpp"
#include <stdarg.h>

class LuaAPI
{
public:
	LuaAPI(lua_State *L = NULL);
	~LuaAPI();

	inline int dostring(const char *str) { return luaL_dostring(m_L, str); }
	inline void push(lua_Integer i) { return lua_pushinteger(m_L, i); }
	inline void push(lua_Number n) { return lua_pushnumber(m_L, n); }
	inline void push(bool b) { return lua_pushboolean(m_L, b); }
	inline void push(lua_CFunction f, int n = 0) { return lua_pushcclosure(m_L, f, n); }
	inline void push(void) { return lua_pushnil(m_L); }
	inline const char *push(const char *s) { return lua_pushstring(m_L, s); }
	inline const char *push(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		return lua_pushvfstring(m_L, fmt, args);
	}
	inline void pop(int n = 1) { return lua_pop(m_L, n); }
	inline int checkstack(int n) { return lua_checkstack(m_L, n); }
	inline int gettop(){ return lua_gettop(m_L); }
	inline int loadbuffer(const char *buf, size_t len, const char *name)
	{
		return luaL_loadbuffer(m_L, buf, len, name);
	}
	inline int pcall(int nargs, int nresults, int msgh)
	{
		return lua_pcall(m_L, nargs, nresults, msgh);
	}
	inline void setglobal(const char *name){ return lua_setglobal(m_L, name); }
	inline const char *tostring(int i){ return lua_tostring(m_L, i); }

private:
	lua_State *m_L;

};

#endif
