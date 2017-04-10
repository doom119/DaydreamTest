//
// Created by Doom119 on 17/3/9.
//

#ifndef DAYDREAMTEST_VKDEVICEHOLDER_H
#define DAYDREAMTEST_VKDEVICEHOLDER_H


#include <vulkan/vulkan.h>
#include <vector>

class VkDeviceHolder
{
public:
    VkDeviceHolder(const VkInstance& instance);
    uint32_t getDeviceCount();
    bool isDiscreteGPU(uint32_t handle);

    //if support graphics queue family, return queue family index, otherwise return -1
    int32_t supportGraphicsQueueFamily(uint32_t deviceIndex);

    //if support present queue family, return queue family index, otherwise return -1
    int32_t supportPresentQueueFamily(uint32_t deviceIndex, VkSurfaceKHR surface);

    //if support VK_KHR_SWAPCHAIN, return true, otherwise return false
    bool supportSwapChain(uint32_t deviceIndex);

    bool selectPhysicalDevice(uint32_t deviceIndex);

    //must call selectPhysicalDevice first
    bool selectGraphicsQueueFamily(uint32_t queueFamilyIndex);

    //must call selectPhysicalDevice first
    bool selectPresentQueueFamily(uint32_t queueFamilyIndex);

    void addExtensionName(const char* name);

    const VkPhysicalDevice& getSelectedPhysicalDevice() const;
    VkDevice& getLogicalDevice();
    uint32_t getSelectedGraphicsQueueFamily() const;
    uint32_t getSelectedPresentQueueFamily() const;

    VkQueue & getQueue();

    //must call selectPhysicalDevice first
    bool createLogicalDevice();

    void release();

private:
    void _getPhysicalDeviceProperties(const VkPhysicalDevice&, VkPhysicalDeviceProperties&);
    void _getPhysicalDeviceFeatures(const VkPhysicalDevice&, VkPhysicalDeviceFeatures&);
    void _getPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice&, std::vector<VkQueueFamilyProperties>&);
    void _getPhysicalDeviceExtensionProperties(const VkPhysicalDevice&, std::vector<VkExtensionProperties>&);

    void _dumpPhysicalDeviceProperties(const VkPhysicalDeviceProperties&);
    void _dumpPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures&);
    void _dumpPhysicalDeviceQueueFamilyProperties(const VkQueueFamilyProperties&);
    void _dumpPhysicalDeviceExtensionProperties(const VkExtensionProperties&);

private:
    bool mIsInited;

    int32_t mSelectedPhysicalDeviceIndex;
    int32_t mSelectedGraphicsQueueFamilyIndex;
    int32_t mSelectedPresentQueueFamilyIndex;
    std::vector<const char*> mExtensionNames;

    std::vector<VkPhysicalDevice> mPhysicalDevices;
    VkPhysicalDevice mSelectedPhysicalDevice;

    VkDevice mLogicalDevice;
    VkQueue mLogicalDeviceQueue;

//    VkPhysicalDeviceProperties mSelectedPhysicalDeviceProperties;
//    VkPhysicalDeviceFeatures mSelectedPhysicalDeviceFeatures;

};


#endif //DAYDREAMTEST_VKDEVICEHOLDER_H
