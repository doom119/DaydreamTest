//
// Created by Doom119 on 17/2/28.
//

#ifndef DAYDREAMTEST_VULKAN_H
#define DAYDREAMTEST_VULKAN_H

#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include "log.h"
#include "VkInstanceHolder.h"

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

    VkInstanceHolder* pInstanceHolder;
private:
    bool _selectPhysicalDevice();
    bool _createLogicDevice();
    bool _getPhysicalDeviceSurfaceCapalities();
    bool _getPhysicalDeviceSurfaceFormats();
    bool _getPhysicalDeviceSurfacePresentModes();

    void _dumpPhysicalDeviceProperties();
    void _dumpPhysicalDeviceSurfaceCapabilities();
    void _dumpPhysicalDeviceSurfaceFormat();
    void _dumpPhysicalDeviceSurfacePresentMode();

public:
    Vulkan():pInstanceHolder(nullptr){}
    bool init();
    bool setSurface(ANativeWindow *window);
    void shutdown();
};


#endif //DAYDREAMTEST_VULKAN_H
