WAYLAND?=/usr/share/wayland
WLROOTS?=/usr/share/wlroots/
CFLAGS?=-Os -pedantic -Wall -g

all:
	$(CC) $(CFLAGS) -I$(WAYLAND)/include -I$(WLROOTS)/include wm.c -L$(WAYLAND)/lib -L$(WLROOTS)/ib -lwayland -lwlroots -o wm

clean:
	rm -f wm
