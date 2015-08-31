# =========================================================================
#      Makefile for cl.exe with mingw32-make edited by CodeSoul
# =========================================================================
#* vim: set filetype=make: *#

# Compiler setting 
OBJECT = DbgEngTest.exe
SOURCE = DbgEngTest.cpp
LIBARARY = msvcrtd.lib kernel32.lib user32.lib gdi32.lib dbgeng.lib
OBJS = $(SOURCE:.cpp=.obj)

CC = cl
LINK = link

PATH_VC12 = E:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
PATH_WINDDK = E:\WinDDK\7600.16385.1
PATH_WINDBG_SDK = E:\Program Files (x86)\Debugging Tools for Windows (x86)\sdk
 
INC_PATH_FLAG = /I"$(PATH_WINDDK)\inc\api" /I"$(PATH_WINDDK)\inc\crt" \
		  /I"$(PATH_WINDBG_SDK)\inc"
LIB_PATH_FLAG = /LIBPATH:"$(PATH_VC12)\lib" /LIBPATH:"$(PATH_WINDDK)\lib\win7\i386" \
		  /LIBPATH:"$(PATH_WINDBG_SDK)\lib\i386"

CFLAGS = /nologo /W3 /Zi /MDd /c $(INC_PATH_FLAG)

ENTRY = /ENTRY:
SUBSYS = /SUBSYSTEM:console
EXPORT = /EXPORT:
LFLAGS = /nologo /PDB:"DbgEngTest.pdb" $(SUBSYS) $(LIB_PATH_FLAG)


.PHONY:$(OBJECT)

$(OBJECT):$(OBJS)
	$(LINK) $(LFLAGS) $< $(LIBARARY) /OUT:$(OBJECT)

%.obj:%.cpp
	$(CC) $(CFLAGS) $<

