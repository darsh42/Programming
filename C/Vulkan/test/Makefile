CC := gcc

CFLAGS := -g
LDFLAGS := -lglfw -lvulkan -lcglm -lpthread -lX11 -lXxf86vm -lXrandr -lXi

BINARY := test

VulkanTest: vulkan_renderer.c darray.c helpers.c
	$(CC) $(CFLAGS) -o $(BINARY) vulkan_renderer.c darray.c helpers.c $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./test

clean:
	rm -f $(BINARY)
