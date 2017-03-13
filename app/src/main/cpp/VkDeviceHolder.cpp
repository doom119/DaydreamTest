//
// Created by Doom119 on 17/3/9.
//

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

bool VkDeviceHolder::selectPhysicalDevice(uint32_t deviceIndex, uint32_t queueFamilyIndex)
{
    if(!mIsInited || deviceIndex >= mPhysicalDevices.size())
    {
        return false;
    }

    mSelectedPhysicalDeviceIndex = deviceIndex;
    VkPhysicalDevice device = mPhysicalDevices.at(deviceIndex);

    _getPhysicalDeviceProperties(device, mSelectedPhysicalDeviceProperties);
    _getPhysicalDeviceFeatures(device, mSelectedPhysicalDeviceFeatures);

    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    _getPhysicalDeviceQueueFamilyProperties(device, queueFamilyProperties);
    mSelectedQueueFamilyProperties = queueFamilyProperties.at(queueFamilyIndex);
    mSelectedQueueFamilyIndex = queueFamilyIndex;

    std::vector<VkExtensionProperties> extensionProperties;
    _getPhysicalDeviceExtensionProperties(device, extensionProperties);
    return true;
}

bool VkDeviceHolder::createLogicalDevice()
{
    float queuePriorities = 1.0f;
    VkDeviceQueueCreateInfo queueCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = static_cast<uint32_t>(mSelectedQueueFamilyIndex),
            .queueCount = 1,
            .pQueuePriorities = &queuePriorities
    };

    VkDeviceCreateInfo deviceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .enabledExtensionCount = 0,
            .ppEnabledExtensionNames = nullptr,
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

    vkGetDeviceQueue(mLogicalDevice, mSelectedQueueFamilyIndex, 0, &mLogicalDeviceQueue);

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
