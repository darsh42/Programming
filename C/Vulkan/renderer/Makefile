CC := gcc

SRC_DIR := ./src
INC_DIR := ./include
BUILD_DIR := ./build

SRCS := $(shell find $(SRC_DIR) -name '*.c')
INCS := $(shell find $(INC_DIR) -name '*.h')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

TARGET := vulkan

CFLAGS := -Wall -Wextra -g
LDFLAGS := -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -r $(BUILD_DIR)
