#include "renderer.h"

renderer_t renderer;

void init_renderer(void) {
    
    assert(SDL_Init(SDL_INIT_VIDEO) == 0);
    assert((renderer.window = SDL_CreateWindow("renderer", 0, 0, 640, 480, SDL_WINDOW_VULKAN)) != NULL);
    
    vulkan_instance_create(&renderer);
    vulkan_physical_pick(&renderer);
    vulkan_device_create(&renderer);
    vulkan_swapchain_create(&renderer);
}
