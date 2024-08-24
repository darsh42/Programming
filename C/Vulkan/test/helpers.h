#pragma once

#include "types.h"
#include "darray.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <vulkan/vulkan_core.h>

// configuration functions
extern void configureInstanceLayers(VkInstanceCreateInfo *ci);
extern void configureInstanceExtensions(VkInstanceCreateInfo *ci);
extern QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);
extern bool check_device_suitablility(
    device_requirements requirements, 
    VkPhysicalDevice device, 
    VkSurfaceKHR surface, 
    const char *extensions[]);

extern vulkan_swapchain_details querySwapchainSupport(VkPhysicalDevice device);

// vulkan API and extension functions
extern VkResult vkCreateDebugUtilsMessengerEXT
(
    VkInstance instance, 
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, 
    const VkAllocationCallbacks *pAllocator, 
    VkDebugUtilsMessengerEXT *pMessenger
);
extern void vkDestroyDebugUtilsMessengerEXT
(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT messenger, 
    const VkAllocationCallbacks *pAllocator
);

extern VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData
);


