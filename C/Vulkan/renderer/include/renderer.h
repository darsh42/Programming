#pragma once

#include "common.h"

#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>

#define VK_CHECK(expr) {assert(expr == VK_TRUE);}

// holds all vulkan state
typedef struct {
    SDL_Window *window;

    VkInstance       instance;
    VkSurfaceKHR     surface;
    VkPhysicalDevice physical_device;
    VkDevice         logical_device;
    VkSwapchainKHR   swapchain;
    
    bool debug;
} renderer_t;

extern void vulkan_instance_create(renderer_t *r, const char *name);
extern void vulkan_physical_pick(renderer_t *r);
extern void vulkan_device_create(renderer_t *r);
extern void vulkan_swapchain_create(renderer_t *r);
