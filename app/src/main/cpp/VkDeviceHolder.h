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
    int32_t supportGraphicsQueueFamily(uint32_t deviceIndex);
    bool selectPhysicalDevice(uint32_t deviceIndex, uint32_t queueFamilyIndex);
    bool createLogicalDevice();

    virtual ~VkDeviceHolder() { vkDestroyDevice(mLogicalDevice, nullptr); }

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
    int32_t mSelectedQueueFamilyIndex;
    VkPhysicalDeviceProperties mSelectedPhysicalDeviceProperties;
    //VkExtensionProperties mSelectedExtensionProperties;
    VkPhysicalDeviceFeatures mSelectedPhysicalDeviceFeatures;
    VkQueueFamilyProperties mSelectedQueueFamilyProperties;
//    std::vector<VkQueueFamilyProperties> mSelectedPhysicalDeviceQueueFamilyProperties;
    std::vector<VkPhysicalDevice> mPhysicalDevices;
    VkDevice mLogicalDevice;
    VkQueue mLogicalDeviceQueue;
};


#endif //DAYDREAMTEST_VKDEVICEHOLDER_H
