PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall -g

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include wm.c -L$(PREFIX)/lib -lX11 -o wm

clean:
	rm -f wm
