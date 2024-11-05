CC := arm-none-eabi-gcc

# Directories
DIR_INCLUDES  := ./includes
DIR_SOURCES   := ./sources
DIR_LINKER    := ./linker
DIR_UTILITIES := ./utilities
DIR_BUILD     := ./build

# Files
TARGET    := $(DIR_BUILD)/stm32.bin
INCLUDES  := -I$(DIR_INCLUDES)
SOURCES_C := $(wildcard $(DIR_SOURCES)/*.c)
SOURCES_S := $(wildcard $(DIR_SOURCES)/*.s)
OBJECTS   := $(patsubst $(DIR_SOURCES)/%.c, $(DIR_BUILD)/%.o, $(SOURCES_C)) \
             $(patsubst $(DIR_SOURCES)/%.s, $(DIR_BUILD)/%.o, $(SOURCES_S))
LINKER    := $(DIR_LINKER)/linker.ld

# Compiler Options
WARNINGS  := -Wall -Wextra
IGNORES   := -Wno-unused-parameter
LIBRARIES :=
DEBUG     :=
FLAGS     := -x assembler-with-cpp -O0

CFLAGS    := $(WARNINGS) $(IGNORES) $(LIBRARIES) -mcpu=cortex-m4 -mthumb
LDFLAGS   :=  -lgcc -T$(LINKER) -mcpu=cortex-m4 -mthumb --specs=nosys.specs -nostdlib

# Create build directory
$(shell mkdir -p $(DIR_BUILD))

# Link objects to elf binary
$(TARGET): $(OBJECTS)
	@echo "Linking $@"
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# Compile C sources to objects
$(DIR_BUILD)/%.o: $(DIR_SOURCES)/%.c
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Compile Assembly sources to objects
$(DIR_BUILD)/%.o: $(DIR_SOURCES)/%.s
	@echo "Assembling $<"
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean flash debug nm

flash:
	@echo "Flashing to target..."

debug:
	@echo "Starting the debug session..."

nm:
	@echo "Listing elf symbols..."
	arm-none-eabi-nm $(TARGET)

clean:
	@echo "Cleaning build directory..."
	rm -rf $(DIR_BUILD)
