#include "types.h"
#include "helpers.h"

void configureInstanceLayers(VkInstanceCreateInfo *ci) {
#ifdef _DEBUG
    const char *validation[] = {
        VALIDATIONLAYERS,
    };

    // check if layers are supported
    {
        uint32_t count = 0;
        VK_CHECK(vkEnumerateInstanceLayerProperties(&count, NULL));
        VkLayerProperties available[count];
        VK_CHECK(vkEnumerateInstanceLayerProperties(&count, available));

        for (int vi = 0; validation[vi]; vi++) {
            bool found = false;

            for (int ai = 0; ai < count; ai++) 
                if (!strcmp(validation[vi], available[ai].layerName)) {
                    found = true;
                    break;
                }
            
            if (!found) {
                fprintf(stderr, "[ERROR]: Validation layer \"%s\" not found\n", validation[vi]);
                ci->enabledLayerCount   = 0;
                ci->ppEnabledLayerNames = NULL;
                return;
            }
        }
    }

    // create name list
    darray_t *layers = darray_create(1);

    for (int i = 0; validation[i]; i++)
        darray_insert(layers, validation[i]);

    ci->enabledLayerCount   = darray_length(layers);
    ci->ppEnabledLayerNames = darray_contents(layers);
#else
    ci->enabledLayerCount   = 0;
    ci->ppEnabledLayerNames = NULL;
#endif
}

void configureInstanceExtensions(VkInstanceCreateInfo *ci) {

    darray_t *extensions = darray_create(1);

    uint32_t glfwCount;   
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwCount);

    for (int i = 0; i < glfwCount; i++)
        darray_insert(extensions, glfwExtensions[i]);
    
#ifdef _DEBUG
    uint32_t validationCount;
    const char *validationExtenstion[] = { VALIDATIONEXTENSIONS };

    validationCount = sizeof(validationExtenstion);

    for (int i = 0; validationExtenstion[i]; i++)
        darray_insert(extensions, validationExtenstion[i]);
#endif

    ci->enabledExtensionCount = darray_length(extensions);
    ci->ppEnabledExtensionNames = darray_contents(extensions);
}

QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface) {
    QueueFamilyIndicies indicies = {-1, -1, -1};

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    VkQueueFamilyProperties queueFamilyProperties[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyProperties);
    
    for (int i = 0; i < queueFamilyCount; i++) {
        VkBool32 present_family_support;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &present_family_support);
        VkQueueFamilyProperties qfp = queueFamilyProperties[i];

        if (qfp.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
            indicies.graphics_family_index = i;
        
        if (present_family_support)
            indicies.present_family_index = i;
    }

    return indicies;
}

bool check_device_suitablility(device_requirements requirements, VkPhysicalDevice device, VkSurfaceKHR surface, const char *extensions[]) {
    VkPhysicalDeviceFeatures   features;
    VkPhysicalDeviceProperties properties;

    vkGetPhysicalDeviceFeatures(device, &features);
    vkGetPhysicalDeviceProperties(device, &properties);

    switch (properties.deviceType) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER: 
            if (!requirements.other_type) 
                return false; 
            break;
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            if (!requirements.integrated_gpu_type) 
                return false; 
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            if (!requirements.discrete_gpu_type) 
                return false; 
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            if (!requirements.virtual_gpu_type) 
                return false; 
            break;
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            if (!requirements.cpu_type) 
                return false; 
            break;
        default: return false;
    }

    // compare device features, if a feature is requested then it must be enabled
    if (requirements.features.robustBufferAccess                      && !(features.robustBufferAccess))                      return false;
    if (requirements.features.fullDrawIndexUint32                     && !(features.fullDrawIndexUint32))                     return false;
    if (requirements.features.imageCubeArray                          && !(features.imageCubeArray))                          return false;
    if (requirements.features.independentBlend                        && !(features.independentBlend))                        return false;
    if (requirements.features.geometryShader                          && !(features.geometryShader))                          return false;
    if (requirements.features.tessellationShader                      && !(features.tessellationShader))                      return false;
    if (requirements.features.sampleRateShading                       && !(features.sampleRateShading))                       return false;
    if (requirements.features.dualSrcBlend                            && !(features.dualSrcBlend))                            return false;
    if (requirements.features.logicOp                                 && !(features.logicOp))                                 return false;
    if (requirements.features.multiDrawIndirect                       && !(features.multiDrawIndirect))                       return false;
    if (requirements.features.drawIndirectFirstInstance               && !(features.drawIndirectFirstInstance))               return false;
    if (requirements.features.depthClamp                              && !(features.depthClamp))                              return false;
    if (requirements.features.depthBiasClamp                          && !(features.depthBiasClamp))                          return false;
    if (requirements.features.fillModeNonSolid                        && !(features.fillModeNonSolid))                        return false;
    if (requirements.features.depthBounds                             && !(features.depthBounds))                             return false;
    if (requirements.features.wideLines                               && !(features.wideLines))                               return false;
    if (requirements.features.largePoints                             && !(features.largePoints))                             return false;
    if (requirements.features.alphaToOne                              && !(features.alphaToOne))                              return false;
    if (requirements.features.multiViewport                           && !(features.multiViewport))                           return false;
    if (requirements.features.samplerAnisotropy                       && !(features.samplerAnisotropy))                       return false;
    if (requirements.features.textureCompressionETC2                  && !(features.textureCompressionETC2))                  return false;
    if (requirements.features.textureCompressionASTC_LDR              && !(features.textureCompressionASTC_LDR))              return false;
    if (requirements.features.textureCompressionBC                    && !(features.textureCompressionBC))                    return false;
    if (requirements.features.occlusionQueryPrecise                   && !(features.occlusionQueryPrecise))                   return false;
    if (requirements.features.pipelineStatisticsQuery                 && !(features.pipelineStatisticsQuery))                 return false;
    if (requirements.features.vertexPipelineStoresAndAtomics          && !(features.vertexPipelineStoresAndAtomics))          return false;
    if (requirements.features.fragmentStoresAndAtomics                && !(features.fragmentStoresAndAtomics))                return false;
    if (requirements.features.shaderTessellationAndGeometryPointSize  && !(features.shaderTessellationAndGeometryPointSize))  return false;
    if (requirements.features.shaderImageGatherExtended               && !(features.shaderImageGatherExtended))               return false;
    if (requirements.features.shaderStorageImageExtendedFormats       && !(features.shaderStorageImageExtendedFormats))       return false;
    if (requirements.features.shaderStorageImageMultisample           && !(features.shaderStorageImageMultisample))           return false;
    if (requirements.features.shaderStorageImageReadWithoutFormat     && !(features.shaderStorageImageReadWithoutFormat))     return false;
    if (requirements.features.shaderStorageImageWriteWithoutFormat    && !(features.shaderStorageImageWriteWithoutFormat))    return false;
    if (requirements.features.shaderUniformBufferArrayDynamicIndexing && !(features.shaderUniformBufferArrayDynamicIndexing)) return false;
    if (requirements.features.shaderSampledImageArrayDynamicIndexing  && !(features.shaderSampledImageArrayDynamicIndexing))  return false;
    if (requirements.features.shaderStorageBufferArrayDynamicIndexing && !(features.shaderStorageBufferArrayDynamicIndexing)) return false;
    if (requirements.features.shaderStorageImageArrayDynamicIndexing  && !(features.shaderStorageImageArrayDynamicIndexing))  return false;
    if (requirements.features.shaderClipDistance                      && !(features.shaderClipDistance))                      return false;
    if (requirements.features.shaderCullDistance                      && !(features.shaderCullDistance))                      return false;
    if (requirements.features.shaderFloat64                           && !(features.shaderFloat64))                           return false;
    if (requirements.features.shaderInt64                             && !(features.shaderInt64))                             return false;
    if (requirements.features.shaderInt16                             && !(features.shaderInt16))                             return false;
    if (requirements.features.shaderResourceResidency                 && !(features.shaderResourceResidency))                 return false;
    if (requirements.features.shaderResourceMinLod                    && !(features.shaderResourceMinLod))                    return false;
    if (requirements.features.sparseBinding                           && !(features.sparseBinding))                           return false;
    if (requirements.features.sparseResidencyBuffer                   && !(features.sparseResidencyBuffer))                   return false;
    if (requirements.features.sparseResidencyImage2D                  && !(features.sparseResidencyImage2D))                  return false;
    if (requirements.features.sparseResidencyImage3D                  && !(features.sparseResidencyImage3D))                  return false;
    if (requirements.features.sparseResidency2Samples                 && !(features.sparseResidency2Samples))                 return false;
    if (requirements.features.sparseResidency4Samples                 && !(features.sparseResidency4Samples))                 return false;
    if (requirements.features.sparseResidency8Samples                 && !(features.sparseResidency8Samples))                 return false;
    if (requirements.features.sparseResidency16Samples                && !(features.sparseResidency16Samples))                return false;
    if (requirements.features.sparseResidencyAliased                  && !(features.sparseResidencyAliased))                  return false;
    if (requirements.features.variableMultisampleRate                 && !(features.variableMultisampleRate))                 return false;
    if (requirements.features.inheritedQueries                        && !(features.inheritedQueries))                        return false;


    // check for required extensions
    uint32_t extension_property_count;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_property_count, NULL);
    VkExtensionProperties extension_properties[extension_property_count];
    vkEnumerateDeviceExtensionProperties(device, NULL, &extension_property_count, extension_properties);

    // string compare extension names
    for (int ri = 0; extensions[ri]; ri++) {
        bool extension_present = false;

        for(int ai = 0; ai < extension_property_count; ai++) {
            if (!strcmp(extensions[ri], extension_properties[ai].extensionName)) {
                extension_present = true;
                break;
            }
        }

        if (!extension_present) {
            fprintf(stderr, "[ERROR] Device does not support required extension %s\n", requirements.extensions[ri]);
            return false;
        }
    }

    // check for queue family support
    QueueFamilyIndicies queueFamilyIndicies = findQueueFamilies(device, surface);
    
    if (requirements.present_queue && queueFamilyIndicies.present_family_index == -1)
        return false;

    if (requirements.graphics_queue && queueFamilyIndicies.graphics_family_index == -1)
        return false;

    if (requirements.compute_queue && queueFamilyIndicies.compute_family_index == -1)
        return false;

    printf("[LOG]: Selected physical device\n");
    printf("\tDevice ID: %d\n", properties.deviceID);
    printf("\tDevice Name: %s\n", properties.deviceName);
    return true;
}

extern vulkan_swapchain_details querySwapchainSupport(VkPhysicalDevice device) {
    vulkan_swapchain_details details;



}


// vulkan API and extension functions
#ifdef _DEBUG
VkResult vkCreateDebugUtilsMessengerEXT
(
    VkInstance instance, 
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, 
    const VkAllocationCallbacks *pAllocator, 
    VkDebugUtilsMessengerEXT *pMessenger
) 
{
    PFN_vkCreateDebugUtilsMessengerEXT func =  (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (func == NULL)
        return VK_ERROR_EXTENSION_NOT_PRESENT;

    return func(instance, pCreateInfo, pAllocator, pMessenger);
}

void vkDestroyDebugUtilsMessengerEXT
(
    VkInstance instance, 
    VkDebugUtilsMessengerEXT messenger, 
    const VkAllocationCallbacks *pAllocator
)
{
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if(func == NULL)
        return;

    func(instance, messenger, pAllocator);
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback
(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData
) 
{
    const char *severity;

    switch (messageSeverity) {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:        severity = "DIAGNOSTIC"; break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:           severity = "INFO";    break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:        severity = "WARNING"; break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:          severity = "ERROR";   break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT: severity = "EXTREEM"; break;
    }
    
    const char *type;

    switch (messageType) {
        case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:     type = "UNRELATED"; break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:  type = "SPECIFICATION"; break;
        case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT: type = "PERFORMANCE"; break;
    }
    
    fprintf(stderr, "[%s]: VIOLATION: {%s}     %s\n", severity, type, pCallbackData->pMessage);

    return VK_FALSE;
}
#endif








