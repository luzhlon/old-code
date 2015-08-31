
#Make tools
CC = cl
LINK = link
MAKE = mingw32-make

#Needful Library
LIBARARY = kernel32.lib user32.lib gdi32.lib

#Path setting
PATH_VC12 = C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC
PATH_WINDDK = E:\WinDDK\7600.16385.1
PATH_WINSDK = E:\Program Files (x86)\Windows Kits\8.1
PATH_LUA = D:\BaiDuYun\Tools\Lua\5.1

#Flags about Path 
INC_PATH_FLAG := /I"$(PATH_WINSDK)\include\shared" /I"$(PATH_WINSDK)\include\um" \
		  /I"$(PATH_LUA)\include" /I"$(PATH_VC12)\include"
LIB_PATH_FLAG := /LIBPATH:"$(PATH_VC12)\lib" /LIBPATH:"$(PATH_WINSDK)\lib\winv6.3\um\x86" \
	/LIBPATH:"$(PATH_LUA)\lib" 

#Link optional flags
ENTRY_ = /ENTRY:
SUBSYS_ = /SUBSYSTEM:
EXPORT_ = /EXPORT:
DLL = /DLL

#Make Config
CONFIG ?= Debug
ifeq "$(CONFIG)" "Debug"
OUTPUT_DIR = Debug
#Compile flags
CFLAGS := /nologo /W3 /Zi /D_DEBUG /MDd /c /Fd $(INC_PATH_FLAG)
LFLAGS := /nologo $(LIB_PATH_FLAG)
endif

ifeq "$(CONFIG)" "Release"
OUTPUT_DIR = Release
#Compile flags
CFLAGS := /nologo /W3 /Zi /MD /c $(INC_PATH_FLAG)
LFLAGS := /nologo $(LIB_PATH_FLAG)
endif

