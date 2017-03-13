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
    int32_t supportGraphicsQueueFamily(uint32_t handle);
    bool selectPhysicalDevice(uint32_t deviceHandle, uint32_t queueFamilyHandle);

private:
    const VkPhysicalDeviceProperties& _getPhysicalDeviceProperties(const VkPhysicalDevice& device);
    const VkPhysicalDeviceFeatures& _getPhysicalDeviceFeatures(const VkPhysicalDevice& device);
    const std::vector<VkQueueFamilyProperties>& _getPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice& device);

    void _dumpPhysicalDeviceProperties(const VkPhysicalDeviceProperties& properties);
    void _dumpPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& features);
    void _dumpPhysicalDeviceQueueFamilyProperties(const VkQueueFamilyProperties& properties);

private:
    bool mIsInited;
    uint32_t mSelectedPhysicalDevice;
    VkPhysicalDeviceProperties mSelectedPhysicalDeviceProperties;
    VkPhysicalDeviceFeatures mSelectedPhysicalDeviceFeatures;
    VkQueueFamilyProperties mSelectedQueueFamilyProperties;
    std::vector<VkQueueFamilyProperties> mSelectedPhysicalDeviceQueueFamilyProperties;
    std::vector<VkPhysicalDevice> mPhysicalDevices;
};


#endif //DAYDREAMTEST_VKDEVICEHOLDER_H
