##
# Example GTK App
#
# @file
# @version 0.1

CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3)

SRC = main.c example.c exampleappwin.c

OBJS = $(SRC:.c=.o)

all: exampleapp

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

exampleapp: $(OBJS)
	$(CC) -o $(@F) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f exampleapp

# end
