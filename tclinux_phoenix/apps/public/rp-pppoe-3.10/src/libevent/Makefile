# Generated automatically from Makefile.in by configure.
# $Id: //BBN_Linux/Branch/Branch_for_Rel_CMCC_7526_20161014/tclinux_phoenix/apps/public/rp-pppoe-3.10/src/libevent/Makefile#1 $
#
# Makefile for event-handling library
#
# Copyright 2002 Roaring Penguin Software Inc.
#
# This software may be distributed according to the terms of the GNU
# General Public License, version 2 or (at your option) any later version.
# LIC: GPL

OBJS=event.o event_tcp.o hash.o event_sig.o
SRCS=$(OBJS:.o=.c)
HDRS=event.h event_tcp.h eventpriv.h hash.h
CFLAGS=-g -O2 -Wall -Wstrict-prototypes -I.. $(DEFINES)
AR=mips-linux-ar

all: libevent.a

libevent.a: $(OBJS)
	rm -f libevent.a
	$(AR) -cq libevent.a $(OBJS)
	mips-linux-ranlib libevent.a

event.o: event.c $(HDRS)
	$(CC) $(CFLAGS) -c -o event.o event.c

hash.o: hash.c $(HDRS)
	$(CC) $(CFLAGS) -c -o hash.o hash.c

event_sig.o: event_sig.c $(HDRS)
	$(CC) $(CFLAGS) -c -o event_sig.o event_sig.c

event_tcp.o: event_tcp.c $(HDRS)
	$(CC) $(CFLAGS) -c -o event_tcp.o event_tcp.c

clean: FORCE
	rm -f *.a *.o *~

FORCE:

.phony: FORCE
