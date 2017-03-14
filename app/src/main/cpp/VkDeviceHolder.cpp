//
// Created by Doom119 on 17/3/9.
//

#include <memory.h>
#include <string>
#include "VkDeviceHolder.h"
#include "log.h"

VkDeviceHolder::VkDeviceHolder(const VkInstance& instance):
        mIsInited(false),mSelectedPhysicalDeviceIndex(-1)
{
    VkResult result;
    uint32_t deviceCount = 0;

    result = vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGW("Enumerate Physical Devices Failed, result=%d", result);
    }
    else
    {
        mPhysicalDevices.resize(deviceCount);
        result = vkEnumeratePhysicalDevices(instance, &deviceCount, mPhysicalDevices.data());
        if(VK_SUCCESS != result)
        {
            LOGW("Get Physical Devices Failed, result=%d", result);
            return;
        }

        mIsInited = true;

#ifdef DEBUG
        for(const auto& device : mPhysicalDevices)
        {
            VkPhysicalDeviceProperties properties;
            _getPhysicalDeviceProperties(device, properties);
            _dumpPhysicalDeviceProperties(properties);

            VkPhysicalDeviceFeatures features;
            _getPhysicalDeviceFeatures(device, features);
            _dumpPhysicalDeviceFeatures(features);

            std::vector<VkQueueFamilyProperties> queueProperties;
            _getPhysicalDeviceQueueFamilyProperties(device, queueProperties);
            for(const auto& queueProperty : queueProperties)
            {
                _dumpPhysicalDeviceQueueFamilyProperties(queueProperty);
            }
        }
#endif
    }
}

uint32_t VkDeviceHolder::getDeviceCount()
{
    if(!mIsInited)
    {
        return 0;
    }

    return mPhysicalDevices.size();
}

bool VkDeviceHolder::isDiscreteGPU(uint32_t index)
{
    if(!mIsInited)
    {
        return false;
    }

    VkPhysicalDeviceProperties properties;
    _getPhysicalDeviceProperties(mPhysicalDevices.at(index), properties);
    if(properties.deviceType & VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        return true;
    }
    return false;
}

int32_t VkDeviceHolder::supportGraphicsQueueFamily(uint32_t deviceIndex)
{
    if(!mIsInited)
    {
        return -1;
    }

    std::vector<VkQueueFamilyProperties> properties;
    _getPhysicalDeviceQueueFamilyProperties(mPhysicalDevices.at(deviceIndex), properties);
    for(int i = 0; i < properties.size(); ++i)
    {
        if(properties.at(i).queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
        {
            return i;
        }
    }

    return -1;
}

int32_t VkDeviceHolder::supportPresentQueueFamily(uint32_t deviceIndex, VkSurfaceKHR surface)
{
    if(!mIsInited)
    {
        return -1;
    }

    std::vector<VkQueueFamilyProperties> properties;
    _getPhysicalDeviceQueueFamilyProperties(mPhysicalDevices.at(deviceIndex), properties);
    VkBool32 isSupport = false;
    for(int i = 0; i < properties.size(); ++i)
    {
        VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevices.at(deviceIndex), i, surface, &isSupport);
        if(VK_SUCCESS != result)
        {
            LOGW("Get Physical Device %d Surface Support Failed, result=%d", i, result);
        }
        else if(isSupport)
        {
            return i;
        }
    }

    return -1;
}


bool VkDeviceHolder::supportKHRSwapChain(uint32_t deviceIndex)
{
    std::vector<VkExtensionProperties> properties;
    _getPhysicalDeviceExtensionProperties(mPhysicalDevices.at(deviceIndex), properties);
    for(const auto& property : properties)
    {
        if(!strcmp(property.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME))
        {
            return true;
        }
    }
    return false;
}

bool VkDeviceHolder::selectPhysicalDevice(uint32_t deviceIndex)
{
    if(!mIsInited || deviceIndex >= mPhysicalDevices.size())
    {
        return false;
    }

    mSelectedPhysicalDevice = mPhysicalDevices.at(deviceIndex);
    mSelectedPhysicalDeviceIndex = deviceIndex;

//    _getPhysicalDeviceProperties(mSelectedPhysicalDevice, mSelectedPhysicalDeviceProperties);
//    _getPhysicalDeviceFeatures(mSelectedPhysicalDevice, mSelectedPhysicalDeviceFeatures);

    return true;
}

bool VkDeviceHolder::selectGraphicsQueueFamily(uint32_t queueFamilyIndex)
{
    if(!mIsInited)
    {
        return false;
    }

    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    _getPhysicalDeviceQueueFamilyProperties(mSelectedPhysicalDevice, queueFamilyProperties);
    if(queueFamilyIndex >= queueFamilyProperties.size())
    {
        return false;
    }
    mSelectedGraphicsQueueFamilyIndex = queueFamilyIndex;
    return true;
}

bool VkDeviceHolder::selectPresentQueueFamily(uint32_t queueFamilyIndex)
{
    if(!mIsInited)
    {
        return false;
    }

    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    _getPhysicalDeviceQueueFamilyProperties(mSelectedPhysicalDevice, queueFamilyProperties);
    if(queueFamilyIndex >= queueFamilyProperties.size())
    {
        return false;
    }
    mSelectedPresentQueueFamilyIndex = queueFamilyIndex;
    return true;
}

void VkDeviceHolder::addExtensionName(const char* name)
{
    if(nullptr == name)
    {
        return;
    }

    mExtensionNames.push_back(name);
}


bool VkDeviceHolder::createLogicalDevice()
{
    float queuePriorities = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = static_cast<uint32_t>(mSelectedGraphicsQueueFamilyIndex),
            .queueCount = 1,
            .pQueuePriorities = &queuePriorities
    };

    VkDeviceCreateInfo deviceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .enabledExtensionCount = static_cast<uint32_t>(mExtensionNames.size()),
            .ppEnabledExtensionNames = mExtensionNames.data(),
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .pEnabledFeatures = nullptr,
            .pQueueCreateInfos = &queueCreateInfo,
            .queueCreateInfoCount = 1
    };
    VkResult result = vkCreateDevice(mPhysicalDevices.at(mSelectedPhysicalDeviceIndex), &deviceCreateInfo, nullptr, &mLogicalDevice);
    if(VK_SUCCESS != result)
    {
        LOGE("Create Logical Device Failed, result=%d", result);
        return false;
    }

    vkGetDeviceQueue(mLogicalDevice, mSelectedGraphicsQueueFamilyIndex, 0, &mLogicalDeviceQueue);

    return true;
}

void VkDeviceHolder::_getPhysicalDeviceProperties(
        const VkPhysicalDevice& device, VkPhysicalDeviceProperties& properties)
{
    vkGetPhysicalDeviceProperties(device, &properties);
}

void VkDeviceHolder::_getPhysicalDeviceFeatures(
        const VkPhysicalDevice& device, VkPhysicalDeviceFeatures& features)
{
    vkGetPhysicalDeviceFeatures(device, &features);
}

void VkDeviceHolder::_getPhysicalDeviceQueueFamilyProperties(
        const VkPhysicalDevice& device, std::vector<VkQueueFamilyProperties>& properties)
{
    uint32_t familyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
    properties.resize(familyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, properties.data());
}

void VkDeviceHolder::_getPhysicalDeviceExtensionProperties(
        const VkPhysicalDevice& device, std::vector<VkExtensionProperties>& properties)
{
    uint32_t count = 0;
    VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &count, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGW("Enumerate Physical Device Extension Properties Failed, result=%d", result);
        return;
    }

    properties.resize(count);
    result = vkEnumerateDeviceExtensionProperties(device, nullptr, &count, properties.data());
    if(VK_SUCCESS != result)
    {
        LOGW("Get Physical Device Extension Properteis Failed, result=%d", result);
        return;
    }

#ifdef DEBUG
    for(const auto& property : properties)
    {
        _dumpPhysicalDeviceExtensionProperties(property);
    }
#endif
}

void VkDeviceHolder::_dumpPhysicalDeviceProperties(
        const VkPhysicalDeviceProperties &properties)
{
    LOGD("Physical Device Properties:\n");
    LOGD("\tDeviceName: %s", properties.deviceName);
    LOGD("\tDeviceId: %d", properties.deviceID);
    LOGD("\tDeviceType: %d", properties.deviceType);
    LOGD("\tDriverVersion: %d", properties.driverVersion);
    LOGD("\tVenderId: %d", properties.vendorID);
    LOGD("\tDevice apiVersion: %d.%d.%d",
         VK_VERSION_MAJOR(properties.apiVersion),
         VK_VERSION_MINOR(properties.apiVersion),
         VK_VERSION_PATCH(properties.apiVersion));

    LOGD("\tin limits:\n");
    LOGD("\t\tmaxImageDimension2D: %d", properties.limits.maxImageDimension2D);
    LOGD("\t\tmaxImageDimension3D: %d", properties.limits.maxImageDimension3D);
    //TODO more in limits

    LOGD("\tin sparseProperties:\n");
    LOGD("\t\tresidencyAlignedMipSize: %d", properties.sparseProperties.residencyAlignedMipSize);
    LOGD("\t\tresidencyNonResidentStrict: %d", properties.sparseProperties.residencyNonResidentStrict);
    LOGD("\t\tresidencyStandard2DBlockShape: %d", properties.sparseProperties.residencyStandard2DBlockShape);
    LOGD("\t\tresidencyStandard3DBlockShape: %d", properties.sparseProperties.residencyStandard3DBlockShape);
    LOGD("\t\tresidencyStandard2DMultisampleBlockShape: %d", properties.sparseProperties.residencyStandard2DMultisampleBlockShape);
}

void VkDeviceHolder::_dumpPhysicalDeviceFeatures(
        const VkPhysicalDeviceFeatures &features)
{
    LOGD("Physical Device Features:\n");
    LOGD("\talphaToOne: %d", features.alphaToOne);
    LOGD("\tdepthBiasClamp: %d", features.depthBiasClamp);
    LOGD("\tgeometryShader: %d", features.geometryShader);
    //TODO more features
}

void VkDeviceHolder::_dumpPhysicalDeviceQueueFamilyProperties(
        const VkQueueFamilyProperties& properties)
{
    LOGD("Physical Device Queue Family Properties:\n");
    LOGD("\tqueueCount: %d", properties.queueCount);
    LOGD("\tqueueFlags: %d", properties.queueFlags);
    LOGD("\ttimestampValidBits: %d", properties.timestampValidBits);
    LOGD("\tminImageTransferGranularity.width: %d", properties.minImageTransferGranularity.width);
    LOGD("\tminImageTransferGranularity.height: %d", properties.minImageTransferGranularity.height);
    LOGD("\tminImageTransferGranularity.depth: %d", properties.minImageTransferGranularity.depth);
}

void VkDeviceHolder::_dumpPhysicalDeviceExtensionProperties(const VkExtensionProperties& properties)
{
    LOGD("Physical Device Extension Properties:\n");
    LOGD("\textensionName: %s", properties.extensionName);
    LOGD("\tspecVersion: %d", properties.specVersion);
}
