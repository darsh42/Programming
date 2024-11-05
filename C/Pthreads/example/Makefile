CC := gcc

DIR_BLD := build
DIR_SRC := src
DIR_INC := include

TARGET  := example
INCLUDE := $(wildcard $(DIR_INC)/*.h)
SOURCES := $(wildcard $(DIR_SRC)/*.c)
OBJECTS := $(patsubst $(DIR_SRC)/%.c, $(DIR_BLD)/%.o, $(SOURCES))
LIBRARY := -pthread -lncurses

vpath %.c $(DIR_SRC)
vpath %.h $(DIR_INC)

CFLAGS := -g -Wall -Wextra

$(DIR_BLD)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBRARY)

$(DIR_BLD)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -rf $(DIR_BLD)/$(TARGET)
