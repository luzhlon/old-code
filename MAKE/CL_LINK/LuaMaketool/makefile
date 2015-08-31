# =========================================================================
#      Makefile for cl.exe with mingw32-make edited by CodeSoul
# =========================================================================
#* vim: set filetype=make: *#

# Compiler setting 
OBJECT = maketool_core.dll
SOURCE = maketool_core.c
LIBARARY = kernel32.lib lua51.lib

CC = cl
LINK = link

KITPATH = E:\WinDDK\7600.16385.1
INCLUDE = $(KITPATH)\inc
INCPATH = /I"$(INCLUDE)\api" /I"$(INCLUDE)\crt"
CFLAGS = /nologo /W3 $(INCPATH)

LIB = $(KITPATH)\lib
ENTRY = /ENTRY:
SUBSYS = /SUBSYSTEM:
EXPORT = /EXPORT:
LIBPATH = /LIBPATH:"$(LIB)\win7\i386" /LIBPATH:"E:\Program Files\Microsoft Visual Studio 12.0\VC\lib"
LFLAGS = /nologo /DLL $(LIBPATH) /OUT:$(OBJECT)


.PHONY:PROJECT1

PROJECT1:
	$(CC) $(CFLAGS) $(SOURCE) /link $(LFLAGS) $(LIBARARY)

