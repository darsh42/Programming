CC=gcc

DIR_SOURCE  := source
DIR_INCLUDE := include
DIR_BUILD   := build

INCLUDE := -I$(DIR_INCLUDE)
SOURCES := $(wildcard $(DIR_SOURCE)/*.c)
OBJECTS := $(patsubst $(DIR_SOURCE)/%.c,$(DIR_BUILD)/%.o,$(SOURCES))
TARGET  := one-pass

CFLAGS  := $(INCLUDE)

$(shell mkdir -p $(DIR_BUILD))

$(DIR_BUILD)/$(TARGET): $(OBJECTS)
	@echo $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBRARIES)

$(DIR_BUILD)/%.o: $(DIR_SOURCE)/%.c
	$(CC) $(CFLAGS) -o $@ -c $< 

.PHONY: clean run debug

clean:
	rm -rf $(DIR_BUILD)
