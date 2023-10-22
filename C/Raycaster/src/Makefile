##
# Raycaster
#
# @raycaster.c
# @version 0.1

CC = gcc
CFLAGS = -O3 -lSDL2 -lm -Wall -Wextra
DEPS = raycaster.h
OBJS = raycaster.o

DEPS_HELP = raycaster.h
OBJS_HELP = raycaster_help.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

RAYCASTER: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

RAYCASTER_HELP: $(OBJS_HELP)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

# end
