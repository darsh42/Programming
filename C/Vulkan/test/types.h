#pragma once

#include <stdbool.h>
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>

#define _DEBUG
#define VK_CHECK(expr)              \
    {                               \
        assert(expr == VK_SUCCESS); \
    }

#define DEVICEEXTENSIONS \
    VK_KHR_SWAPCHAIN_EXTENSION_NAME, \
    NULL

#define VALIDATIONEXTENSIONS \
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME, \
    NULL

#define VALIDATIONLAYERS \
    "VK_LAYER_KHRONOS_validation", \
    NULL


typedef struct vulkan_queue_family_indicies {
    int32_t graphics_family_index;
    int32_t present_family_index;
    int32_t compute_family_index;
} QueueFamilyIndicies;

typedef struct device_requirements {
    bool other_type;
    bool integrated_gpu_type;
    bool discrete_gpu_type;
    bool virtual_gpu_type;
    bool cpu_type;

    bool graphics_queue;
    bool  present_queue;
    bool  compute_queue;

    const char **extensions;

    VkPhysicalDeviceFeatures   features;

} device_requirements;

typedef struct swapchain_details {
    VkSurfaceCapabilitiesKHR capabilities;
    uint32_t format_counts;
    VkSurfaceFormatKHR      *formats;
    uint32_t present_mode_counts;
    VkSurfacePresentModeEXT *presentModes;
} vulkan_swapchain_details;

// main context handler
typedef struct vulkan_context {
    VkInstance    instance;
    VkSurfaceKHR   surface;
    VkQueue graphics_queue;
    VkQueue  present_queue;

#ifdef _DEBUG
    VkDebugUtilsMessengerEXT debugMessenger;
#endif
} vulkan_context;

// device handlers
typedef struct vulkan_device {
    VkPhysicalDevice physicalDevice;
    VkDevice         logicalDevice;
} vulkan_device;

struct PSX_CONTEXT {
    GLFWwindow *window;

    vulkan_context vContext;
    vulkan_device  vDevice;
};
