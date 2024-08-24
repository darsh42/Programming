#include "common.h"
#include "renderer.h"
#include <time.h>
#include <vulkan/vulkan_core.h>

static const char **load_layers(uint32_t *layer_counts, bool debug);
static const char **load_extensions(uint32_t *extension_counts);

void vulkan_instance_create(renderer_t *r, const char *name) {
    VkInstanceCreateInfo create_info = {};
    VkApplicationInfo       app_info = {};
    
    const char **layer_names, **extension_names;
    uint32_t     layer_counts,  extension_counts;

    layer_names     = load_layers(&layer_counts, r->debug);
    extension_names = load_extensions(&extension_counts);

    app_info.sType =VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName   = name;
    app_info.pEngineName        = "No Engine";
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
    app_info.apiVersion         = VK_API_VERSION_1_0;

    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;
    create_info.ppEnabledLayerNames     = layer_names;
    create_info.enabledLayerCount       = layer_counts;
    create_info.ppEnabledExtensionNames = extension_names; 
    create_info.enabledExtensionCount   = extension_counts;

    VK_CHECK(vkCreateInstance(&create_info, NULL, &r->instance));

    RENDER_LOG(LOG_LEVEL_INFO, "Sucessfully created vulkan instance\n");
}

const char **load_layers(uint32_t *layer_count, bool debug) {
    if (!debug) {
        *layer_count = 0;
        return NULL;
    }
    
    VkLayerProperties *properties;

    assert(vkEnumerateInstanceExtensionProperties(NULL, layer_count, NULL) == VK_SUCCESS);
    assert((properties = malloc(sizeof(VkLayerProperties) * *layer_count)) != NULL);
    assert(vkEnumerateInstanceExtensionProperties(NULL, layer_count, NULL) == VK_SUCCESS);
    
    const char **required_layers;
    assert((required_layers = malloc(sizeof(char *) * 3)) != NULL);

    required_layers = (const char *[]) {
        "VK_LAYER_KHRONOS_validation_layer",
        NULL
    };

    

    return NULL;
}


