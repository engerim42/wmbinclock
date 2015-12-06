#
# wmbinclock Makefile
#
CC     = gcc
CFLAGS = -O2 -Wall
INCDIR = -I/usr/X11R6/include/X11 -I/usr/X11R6/include
DESTDIR= /usr/X11R6
LIBDIR = -L/usr/X11R6/lib
# for linux
LIBS   = -lXpm -lX11 -lXext -lm
# for Solaris
# LIBS   = -lXpm -lX11 -lXext -lsocket
OBJS   = wmBinClock.o \
         xutils.o


.c.o:
	$(CC) $(CFLAGS) -D$(shell echo `uname -s`) -c $< -o $*.o $(INCDIR)


all:	wmBinClock.o wmBinClock

wmBinClock.o: wmBinClock_master.xpm wmBinClock_mask.xbm
wmBinClock:	$(OBJS) 
	$(CC) $(CFLAGS) $(SYSTEM) -o wmBinClock $^ $(INCDIR) $(LIBDIR) $(LIBS)

clean:
	for i in $(OBJS) ; do \
		rm -f $$i; \
	done
	rm -f wmBinClock

install:: wmBinClock
	install -c -s -m 0755 wmBinClock $(DESTDIR)/bin

