# =========================================================================
#      Makefile for cl.exe with mingw32-make edited by CodeSoul
# =========================================================================
#* vim: set filetype=make: *#

# Compiler setting 
OBJECT = LuaDebugLib.dll
SOURCE = LuaDebugLib.cpp 
LIBARARY = msvcrtd.lib kernel32.lib user32.lib gdi32.lib Lua53.lib

CC = cl
LINK = link

VC12PATH = E:\Program Files\Microsoft Visual Studio 12.0\VC
WINDDKPATH = E:\WinDDK\7600.16385.1
WINDBGSDKPATH = E:\Program Files\Debugging Tools for Windows (x86)\sdk
PATH_WIN_SDK = E:\Program Files (x86)\Windows Kits\8.1
 
INCFLAG = /I"$(WINDDKPATH)\inc\api" /I"$(WINDDKPATH)\inc\crt" \
		  /I"$(WINDBGSDKPATH)\inc"
LIBFLAG = /LIBPATH:"$(VC12PATH)\lib" /LIBPATH:"$(WINDDKPATH)\lib\win7\i386" \
		  /LIBPATH:"$(WINDBGSDKPATH)\lib\i386"

CFLAGS = /nologo /W3 /Zi /D_DEBUG /MDd /c $(INCFLAG)

ENTRY = /ENTRY:
SUBSYS = /SUBSYSTEM:
EXPORT = /EXPORT:
LFLAGS = /nologo /DLL $(LIBFLAG)


.PHONY:$(OBJECT)

$(OBJECT):LuaDebugLib.obj
	$(LINK) $(LFLAGS) LuaDebugLib.obj $(LIBARARY) /OUT:$(OBJECT)

LuaDebugLib.obj:LuaDebugLib.cpp
	$(CC) $(CFLAGS) LuaDebugLib.cpp

