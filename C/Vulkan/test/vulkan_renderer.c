#include <time.h>
#include <vulkan/vulkan_core.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <cglm/vec4.h>
#include <cglm/mat4.h>

#include <vulkan/vulkan.h>

#include "types.h"
#include "helpers.h"

#include <stdio.h>
#include <string.h>

void initWindow(struct PSX_CONTEXT *ctx);
void initVulkan(struct PSX_CONTEXT *ctx);
void loop(struct PSX_CONTEXT *ctx);
void quit(struct PSX_CONTEXT *ctx);

#ifdef _DEBUG
void vulkanSetupDebugMessenger(struct PSX_CONTEXT *ctx) {
    VkDebugUtilsMessengerCreateInfoEXT createinfo = {};
    createinfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createinfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | 
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT    | 
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | 
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createinfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT    |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createinfo.pfnUserCallback = debugCallback;
    createinfo.pUserData = NULL;

    VkDebugUtilsMessengerEXT debugMessenger;
    VK_CHECK(vkCreateDebugUtilsMessengerEXT(ctx->vContext.instance, &createinfo, NULL, &debugMessenger));

    printf("[LOG]: Created debug messenger\n");
    ctx->vContext.debugMessenger = debugMessenger;
}
#endif

void vulkanCreateInstance(struct PSX_CONTEXT *ctx) {
    VkApplicationInfo       appinfo = {};
    VkInstanceCreateInfo createinfo = {};
    
    appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appinfo.pApplicationName = "PSX";
    appinfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appinfo.pEngineName = "PSX Engine";
    appinfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appinfo.apiVersion = VK_API_VERSION_1_0;

    createinfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createinfo.pApplicationInfo = &appinfo;

    configureInstanceExtensions(&createinfo);
    configureInstanceLayers(&createinfo);

    VkInstance instance; VkResult result;
    VK_CHECK(vkCreateInstance(&createinfo, NULL, &instance));
    
    printf("[LOG]: Created Vulkan Instance\n");
    ctx->vContext.instance = instance;
}

void vulkanCreateSurface(struct PSX_CONTEXT *ctx) {
    VkSurfaceKHR surface;
    VK_CHECK(glfwCreateWindowSurface(ctx->vContext.instance, ctx->window, NULL, &surface));

    printf("[LOG]: Created Vulkan Surface\n");
    ctx->vContext.surface = surface;
}

void createSuitableDevices(struct PSX_CONTEXT *ctx) {
    // create device requirements
    device_requirements requirements = {};
    requirements.integrated_gpu_type = true;
    requirements.discrete_gpu_type   = true;
    requirements.graphics_queue = true;
    requirements.present_queue  = true;

    requirements.features.geometryShader = VK_TRUE;

    // define require ddevice extensions
    const char *extensions[] = {
        DEVICEEXTENSIONS
    };
    
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(ctx->vContext.instance, &device_count, NULL);
    VkPhysicalDevice devices[device_count];
    vkEnumeratePhysicalDevices(ctx->vContext.instance, &device_count, devices);
    
    for (int i = 0; i < device_count; i++) {
        if (check_device_suitablility(requirements, devices[i], ctx->vContext.surface, extensions)) {
            ctx->vDevice.physicalDevice = devices[i];
            break;
        }
    }

    QueueFamilyIndicies indicies = findQueueFamilies(ctx->vDevice.physicalDevice, ctx->vContext.surface);

    float queue_priority = 1.0f;

    VkDeviceQueueCreateInfo graphicCreateInfo = {};
    graphicCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    graphicCreateInfo.queueFamilyIndex = indicies.graphics_family_index;
    graphicCreateInfo.queueCount = 1;
    graphicCreateInfo.pQueuePriorities = &queue_priority;

    VkDeviceQueueCreateInfo presentCreateInfo = {};
    presentCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    presentCreateInfo.queueFamilyIndex = indicies.present_family_index;
    presentCreateInfo.queueCount = 1;
    presentCreateInfo.pQueuePriorities = &queue_priority;

    const VkDeviceQueueCreateInfo queues[] = {
        graphicCreateInfo,
        presentCreateInfo
    };

    VkPhysicalDeviceFeatures deviceFeatures= {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = queues;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledLayerCount = 0;
        createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = extensions;
    
    VkDevice device;
    VK_CHECK(vkCreateDevice(ctx->vDevice.physicalDevice, &createInfo, NULL, &device));

    printf("[LOG]: Created Logical device\n");
    ctx->vDevice.logicalDevice = device;

    vkGetDeviceQueue(device, indicies.graphics_family_index, 0, &ctx->vContext.graphics_queue);
    vkGetDeviceQueue(device, indicies.present_family_index, 0, &ctx->vContext.present_queue);
}

void vulkanCreateSwapchain(struct PSX_CONTEXT *ctx) {
    vulkan_swapchain_details swapchain_details;

    VkSurfaceFormatKHR    format;
    VkPresentModeKHR presentMode;
    VkExtent2D            extent;

    VkSwapchainCreateInfoKHR createInfo = {};
}

void initVulkan(struct PSX_CONTEXT *ctx) {
    vulkanCreateInstance(ctx);
    vulkanSetupDebugMessenger(ctx);
    vulkanCreateSurface(ctx);
    createSuitableDevices(ctx);
}

void initWindow(struct PSX_CONTEXT *ctx) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    ctx->window = glfwCreateWindow(800, 600, "Vulkan triangle", NULL, NULL);
    
}

void loop(struct PSX_CONTEXT *ctx) {
    while(!glfwWindowShouldClose(ctx->window))
        glfwPollEvents();
}

void quit(struct PSX_CONTEXT *ctx) {
    #ifdef _DEBUG
    vkDestroyDebugUtilsMessengerEXT(ctx->vContext.instance, ctx->vContext.debugMessenger, NULL);
    #endif

    vkDestroyDevice(ctx->vDevice.logicalDevice, NULL);
    vkDestroySurfaceKHR(ctx->vContext.instance, ctx->vContext.surface, NULL);
    vkDestroyInstance(ctx->vContext.instance, NULL);

    glfwDestroyWindow(ctx->window);
    glfwTerminate();
    exit(0);
}

void run(void) {
    struct PSX_CONTEXT context;
    initWindow(&context);
    initVulkan(&context);
    loop(&context);
    quit(&context);
}


int main(void) {
    run();
    return 0;
}
