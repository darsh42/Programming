CC := gcc

DIR_SOURCES	 := sources
DIR_INCLUDES := includes
DIR_BUILD    := build

TARGET   := renderer
SOURCES  := $(wildcard $(DIR_SOURCES)/*.c)
OBJECTS  := $(patsubst $(DIR_SOURCES)/%.c,$(DIR_BUILD)/%.o,$(SOURCES))
INCLUDES := -I$(DIR_INCLUDES)

LIBRARIES := -lSDL2

$(shell mkdir -p $(DIR_BUILD))

$(DIR_BUILD)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@ $(LIBRARIES)

$(DIR_BUILD)/%.o: $(DIR_SOURCES)/%.c
	$(CC) $(INCLUDES) -c $< -o $@ $(LIBRARIES)

.PHONY: clean run debug

clean:
	rm -rf $(DIR_BUILD)

run:
	./$(DIR_BUILD)/$(TARGET)
