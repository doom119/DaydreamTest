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

private:
    VkPhysicalDeviceProperties _getPhysicalDeviceProperties(const VkPhysicalDevice& device);
    VkPhysicalDeviceFeatures _getPhysicalDeviceFeatures(const VkPhysicalDevice& device);
    std::vector<VkQueueFamilyProperties> _getPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice& device);

    void _dumpPhysicalDeviceProperties(const VkPhysicalDeviceProperties& properties);
    void _dumpPhysicalDeviceFeatures(const VkPhysicalDeviceFeatures& features);
    void _dumpPhysicalDeviceQueueFamilyProperties(const VkQueueFamilyProperties& properties);

private:
    std::vector<VkPhysicalDevice> mPhysicalDevices;
};


#endif //DAYDREAMTEST_VKDEVICEHOLDER_H
