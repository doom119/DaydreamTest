//
// Created by Doom119 on 17/2/28.
//

#ifndef DAYDREAMTEST_VULKAN_H
#define DAYDREAMTEST_VULKAN_H

#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include "log.h"

class Vulkan
{
private:
    VkInstance mVkInstance;
    VkPhysicalDevice mVkPhysicalDevice;
    VkDevice mVkLogicalDevice;
    VkSurfaceKHR mSurfaceKHR;
    VkSurfaceCapabilitiesKHR mSurfaceCapabilitiesKHR;
    VkSurfaceFormatKHR mSurfaceFormatKHR;
    VkPresentModeKHR mPresentModeKHR;
private:
    bool _createInstance();
    bool _selectPhysicalDevice();
    bool _createLogicDevice();
    bool _getPhysicalDeviceSurfaceCapalities();
    bool _getPhysicalDeviceSurfaceFormats();
    bool _getPhysicalDeviceSurfacePresentModes();

    void _dumpInstanceExtensionProperties();
    void _dumpPhysicalDeviceProperties();
    void _dumpPhysicalDeviceSurfaceCapabilities();
    void _dumpPhysicalDeviceSurfaceFormat();
    void _dumpPhysicalDeviceSurfacePresentMode();

public:
    bool init();
    bool setSurface(ANativeWindow *window);
};


#endif //DAYDREAMTEST_VULKAN_H
