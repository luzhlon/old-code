# =========================================================================
#      Makefile for cl.exe with mingw32-make edited by CodeSoul
# =========================================================================
#* vim: set filetype=make: *#

# Compiler setting 
OBJECT = Lua53.exe
SOURCE = Lua53.cpp
LIBARARY = msvcrtd.lib kernel32.lib user32.lib gdi32.lib Lua53.lib

CC = cl
LINK = link

VC12PATH = E:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
WINDDKPATH = E:\WinDDK\7600.16385.1
WINDBGSDKPATH = E:\Program Files\Debugging Tools for Windows (x86)\sdk
 
INCFLAG = /I"$(WINDDKPATH)\inc\api" /I"$(WINDDKPATH)\inc\crt"
LIBFLAG = /LIBPATH:"$(VC12PATH)\lib" /LIBPATH:"$(WINDDKPATH)\lib\win7\i386"

CFLAGS = /nologo /W3 /Zi /MDd /c $(INCFLAG)

ENTRY = /ENTRY:
SUBSYS = /SUBSYSTEM:console
EXPORT = /EXPORT:
LFLAGS = /nologo $(SUBSYS) $(LIBFLAG)


.PHONY:$(OBJECT)

$(OBJECT):Lua53.obj
	$(LINK) $(LFLAGS) Lua53.obj $(LIBARARY) /OUT:$(OBJECT)

Lua53.obj:Lua53.cpp
	$(CC) $(CFLAGS) Lua53.cpp

