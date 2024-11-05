CC := gcc

SOURCES     := $(wildcard *.c)
OBJECTS     := $(patsubst %.c,%.o,$(SOURCES))
EXECUTABLES := $(basename $(SOURCES))

CFLAGS := -g -lpthread

$(EXECUTABLES): $(OBJECTS);

%.o: %.c
	$(CC) $< -o $(basename $<) $(CFLAGS)

.PHONY: clean

clean:
	rm -rf $(EXECUTABLES)
