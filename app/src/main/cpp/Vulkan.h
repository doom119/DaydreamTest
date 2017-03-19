//
// Created by Doom119 on 17/2/28.
//

#ifndef DAYDREAMTEST_VULKAN_H
#define DAYDREAMTEST_VULKAN_H

#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include "log.h"
#include "VkInstanceHolder.h"
#include "VkDeviceHolder.h"
#include "VkSurfaceHolder.h"

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
    VkDeviceHolder* pDeviceHolder;
    VkSurfaceHolder* pSurfaceHolder;

public:
    Vulkan():pInstanceHolder(nullptr), pDeviceHolder(nullptr), pSurfaceHolder(nullptr){}
    bool init();
    bool setSurface(ANativeWindow *window);
    void shutdown();
};


#endif //DAYDREAMTEST_VULKAN_H
