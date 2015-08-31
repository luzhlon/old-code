#
# a very simple Makefile for miniLZO
#
# Copyright (C) 1996-2011 Markus F.X.J. Oberhumer
#

CC		=	$(CROSS)gcc
CFLAGS	=	-s -Wall -O2 -fomit-frame-pointer
LDFLAGS	=	
APP		=	lzo_comp lzo_decomp

.PHONY: all clean


all:$(APP)

lzo_comp: comp.o minilzo.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

lzo_decomp: decomp.o minilzo.o
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) *.o ~* $(APP) *.lzo -rf

