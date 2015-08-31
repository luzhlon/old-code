# =========================================================================
#      Makefile for cl.exe with mingw32-make edited by CodeSoul
# =========================================================================
#* vim: set filetype=make: *#

# Compiler setting 
SOURCE = wxLearn.cpp
OBJECT = wxLearn.exe
LIBARARY = msvcrtd.lib kernel32.lib user32.lib gdi32.lib oleaut32.lib Comdlg32.lib \
           Rpcrt4.lib Advapi32.lib ole32.lib shell32.lib comctl32.lib Winspool.lib \
		   wxmsw31ud_core.lib wxbase31ud.lib wxexpatd.lib wxjpegd.lib \
		   wxregexud.lib wxtiffd.lib wxzlibd.lib wxpngd.lib

#Make tools
CC = cl
LINK = link
MAKE = mingw32-make

#Path setting
PATH_VC12 = E:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
PATH_WINDDK = E:\WinDDK\7600.16385.1
PATH_WXWIDGETS = E:\wxWidgets
PATH_WINDBG_SDK = E:\Program Files (x86)\Debugging Tools for Windows (x86)\sdk
PATH_WINSDK = E:\Program Files (x86)\Windows Kits\8.1

#Flags about Path 
INC_PATH_FLAG := /I"$(PATH_VC12)\include" \
      /I"$(PATH_WINSDK)\Include\shared" /I"$(PATH_WINSDK)\Include\um"\
	  /I"$(PATH_WXWIDGETS)\include"  /I"$(PATH_WXWIDGETS)\include\wx\msw" 
LIB_PATH_FLAG := /LIBPATH:"$(PATH_VC12)\lib" /LIBPATH:"$(PATH_WINDDK)\lib\win7\i386" \
	/LIBPATH:"$(PATH_WINDBG_SDK)\lib\i386" /LIBPATH:"$(PATH_WXWIDGETS)\lib\vc_lib" #/LIBPATH:"$(PATH_WXWIDGETS)" /LIBPATH:"$(PATH_WXWIDGETS)"

#Make Config
CONFIG ?= Debug
ifeq "$(CONFIG)" "Debug"
OUTPUT_DIR = Debug
#Compile flags
CFLAGS := /nologo /W3 /Zi /Fd /D_DEBUG /MDd /c $(INC_PATH_FLAG) #/Fo$(OUTPUT_DIR)\wxLearn 
#Link flags
ENTRY = /ENTRY:main
SUBSYS = /SUBSYSTEM:console
EXPORT = /EXPORT:
LFLAGS := /nologo $(LIB_PATH_FLAG) /PDB:$(OBJECT:.cpp=.pdb)
endif

ifeq "$(CONFIG)" "Release"
OUTPUT_DIR = Release
#Compile flags
CFLAGS := /nologo /W3 /Zi /MD /c $(INC_PATH_FLAG)
#Link flags
ENTRY = /ENTRY:main
SUBSYS = /SUBSYSTEM:console
EXPORT = /EXPORT:
LFLAGS := /nologo $(LIB_PATH_FLAG)
endif

OBJS := $(SOURCE:.cpp=.obj)

.PHONY:$(OBJECT)

$(OBJECT):$(OBJS)
	$(LINK) $(LFLAGS) $(OBJS) $(LIBARARY) /OUT:$(OBJECT)

%.obj:%.cpp
	$(CC) $(CFLAGS) $<


