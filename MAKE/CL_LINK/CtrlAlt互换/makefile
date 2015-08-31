# =========================================================================
#      Makefile for cl.exe with mingw32-make edited by CodeSoul
# =========================================================================
#* vim: set filetype=make: *#

# Compiler setting 
SOURCE = ctrlalt.c

include ../MSVC.mak

OBJECT = ctrlalt.exe
OBJS := $(SOURCE:.c=.obj)

.PHONY:$(OBJECT)

$(OBJECT):$(OBJS)
	$(LINK) $(LFLAGS) $(OBJS) $(LIBARARY) /OUT:$(OBJECT)

%.obj:%.c
	$(CC) $(CFLAGS) $<


