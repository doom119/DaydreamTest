//
// Created by Doom119 on 17/2/28.
//

#include <vector>
#include "Vulkan.h"

bool Vulkan::init()
{
//    return _createInstance() && _selectPhysicalDevice() && _createLogicDevice();
    if(nullptr != pInstanceHolder)
    {
        delete pInstanceHolder;
    }
    pInstanceHolder = new VkInstanceHolder();
    const VkInstance& vkInstance = pInstanceHolder->createInstance("VulkanTest", 1, "VulkanTest", 1);
    pDeviceHolder = new VkDeviceHolder(vkInstance);
    int deviceCount = pDeviceHolder->getDeviceCount();
    int selectedDevice = -1;
    int selectedQueueFamily = -1;
    for(int i = 0; i < deviceCount; ++i)
    {
        if((selectedQueueFamily = pDeviceHolder->supportGraphicsQueueFamily(i)) >= 0)
        {
            selectedDevice = i;
            break;
        }
    }
    LOGD("selectedDevice=%d, selectedQueueFamily=%d", selectedDevice, selectedQueueFamily);
    if(selectedDevice >= 0 && selectedQueueFamily >= 0)
    {
        pDeviceHolder->selectPhysicalDevice(selectedDevice, selectedQueueFamily);
        pDeviceHolder->createLogicalDevice();
    }

    return true;
}

void Vulkan::shutdown()
{
    delete pInstanceHolder;
}

bool Vulkan::_getPhysicalDeviceSurfaceCapalities()
{
    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mVkPhysicalDevice, mSurfaceKHR, &mSurfaceCapabilitiesKHR);
    if(VK_SUCCESS != result)
    {
        LOGE("Get Physical Device Surface Capalities Failed, result=%d", result);
        return false;
    }

    _dumpPhysicalDeviceSurfaceCapabilities();
    return true;
}

bool Vulkan::_getPhysicalDeviceSurfaceFormats()
{
    uint32_t formatCount = 0;
    VkResult result;
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(mVkPhysicalDevice, mSurfaceKHR, &formatCount, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGE("Enumerate Physical Device Surface Format Failed, result=%d", result);
        return false;
    }
    LOGD("Physical Device Surface Format Count: %d", formatCount);
    VkSurfaceFormatKHR surfaceFormat[formatCount];
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(mVkPhysicalDevice, mSurfaceKHR, &formatCount, surfaceFormat);
    if(VK_SUCCESS != result)
    {
        LOGE("Get Physical Device Surface Formats Failed, result=%d", result);
        return false;
    }
    mSurfaceFormatKHR = surfaceFormat[0];

    _dumpPhysicalDeviceSurfaceFormat();

    return true;
}

bool Vulkan::_getPhysicalDeviceSurfacePresentModes()
{
    uint32_t modeCount = 0;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(mVkPhysicalDevice,
                                                                mSurfaceKHR,
                                                                &modeCount,
                                                                nullptr);
    if(VK_SUCCESS != result)
    {
        LOGE("Enumerate Physical Device Surface Present Modes Failed, result=%d", result);
        return false;
    }
    LOGD("Physical Device Surface Present Mode Count: %d", modeCount);
    VkPresentModeKHR presentMode[modeCount];
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(mVkPhysicalDevice,
                                                       mSurfaceKHR,
                                                       &modeCount,
                                                       presentMode);
    if(VK_SUCCESS != result)
    {
        LOGE("Get Physical Device Surface Prensent Mode Failed, result=%d", result);
        return false;
    }
    mPresentModeKHR = presentMode[0];

    _dumpPhysicalDeviceSurfacePresentMode();

    return true;
}

bool Vulkan::_createLogicDevice()
{
    float priorities[] = {1.0f,};
    VkDeviceQueueCreateInfo queueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueCount = 1,
            .queueFamilyIndex = 0,
            .pQueuePriorities = priorities
    };

    std::vector<const char *> deviceExt;
    deviceExt.push_back("VK_KHR_swapchain");
    VkDeviceCreateInfo deviceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pQueueCreateInfos = &queueCreateInfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = static_cast<uint32_t >(deviceExt.size()),
            .ppEnabledExtensionNames = deviceExt.data(),
            .pEnabledFeatures = nullptr
    };

    VkResult result = vkCreateDevice(mVkPhysicalDevice, &deviceCreateInfo, nullptr, &mVkLogicalDevice);
    if(result != VK_SUCCESS)
    {
        LOGE("Create Vulkan Logical Device Failed, result=%d", result);
        return false;
    }

    return true;
}

bool Vulkan::setSurface(ANativeWindow *window)
{
    VkAndroidSurfaceCreateInfoKHR surfaceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .window = window
    };
    VkResult result = vkCreateAndroidSurfaceKHR(mVkInstance, &surfaceCreateInfo, nullptr, &mSurfaceKHR);
    if(VK_SUCCESS != result)
    {
        LOGE("Create Android Surface Failed, result=%d", result);
        return false;
    }

    _getPhysicalDeviceSurfaceCapalities();
    _getPhysicalDeviceSurfaceFormats();
    _getPhysicalDeviceSurfacePresentModes();

    LOGD("Create Android Surface Success");
    return true;
}

void Vulkan::_dumpPhysicalDeviceSurfaceCapabilities()
{
    LOGD("Physical Device Surface Capalities:\n");
    LOGD("\tcurrentExtent.width: %d", mSurfaceCapabilitiesKHR.currentExtent.width);
    LOGD("\tcurrentExtent.height: %d", mSurfaceCapabilitiesKHR.currentExtent.height);
    LOGD("\tmaxImageCount= %d", mSurfaceCapabilitiesKHR.maxImageCount);
    LOGD("\tmaxImageExtent.width: %d", mSurfaceCapabilitiesKHR.maxImageExtent.width);
    LOGD("\tmaxImageExtent.height: %d", mSurfaceCapabilitiesKHR.maxImageExtent.height);
    LOGD("\tminImageCount: %d", mSurfaceCapabilitiesKHR.minImageCount);
    LOGD("\tminImageExtent.width: %d", mSurfaceCapabilitiesKHR.minImageExtent.width);
    LOGD("\tminImageExtent.height: %d", mSurfaceCapabilitiesKHR.minImageExtent.height);
    LOGD("\tcurrentTransform: %d", mSurfaceCapabilitiesKHR.currentTransform);
    LOGD("\tmaxImageArrayLayers: %d", mSurfaceCapabilitiesKHR.maxImageArrayLayers);
    LOGD("\tsupportedCompositeAlpha: %d", mSurfaceCapabilitiesKHR.supportedCompositeAlpha);
    LOGD("\tsupportedTransforms: %d", mSurfaceCapabilitiesKHR.supportedTransforms);
    LOGD("\tsupportedUsageFlags: %d", mSurfaceCapabilitiesKHR.supportedUsageFlags);
}

void Vulkan::_dumpPhysicalDeviceSurfaceFormat()
{
    LOGD("Physical Device Surface Format:\n");
    LOGD("\tcolorSpace: %d", mSurfaceFormatKHR.colorSpace);
    LOGD("\tformat: %d", mSurfaceFormatKHR.format);
}

void Vulkan::_dumpPhysicalDeviceSurfacePresentMode()
{
    LOGD("Physical Device Surface Present Mode:\n");
    LOGD("\tpresent mode: %d", mPresentModeKHR);
}

