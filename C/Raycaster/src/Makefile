##
# Raycaster
#
# @raycaster.c
# @version 0.1

CC = gcc
CFLAGS = -g -lSDL2 -lm -Wall -Wextra
DEPS = raycaster.h
OBJS = raycaster.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

RAYCASTER: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o

# end
