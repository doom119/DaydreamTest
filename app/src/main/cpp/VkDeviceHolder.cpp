//
// Created by Doom119 on 17/3/9.
//

#include "VkDeviceHolder.h"
#include "log.h"

VkDeviceHolder::VkDeviceHolder(const VkInstance& instance):mIsInited(false)
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
            _getPhysicalDeviceProperties(device);
            _getPhysicalDeviceFeatures(device);
            _getPhysicalDeviceQueueFamilyProperties(device);
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
    vkGetPhysicalDeviceProperties(mPhysicalDevices.at(index), &properties);
    if(properties.deviceType & VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        return true;
    }
    return false;
}

int32_t VkDeviceHolder::supportGraphicsQueueFamily(uint32_t handle)
{
    if(!mIsInited)
    {
        return -1;
    }

    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevices.at(handle), &count, nullptr);
    std::vector<VkQueueFamilyProperties> properties(count);
    vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevices.at(handle), &count, properties.data());
    for(int i = 0; i < properties.size(); ++i)
    {
        if(properties.at(i).queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)
        {
            return i;
        }
    }

    return -1;
}

bool VkDeviceHolder::selectPhysicalDevice(uint32_t deviceHandle, uint32_t queueFamilyHandle)
{
    if(!mIsInited || deviceHandle >= mPhysicalDevices.size())
    {
        return false;
    }

    mSelectedPhysicalDevice = deviceHandle;
    mSelectedPhysicalDeviceProperties = _getPhysicalDeviceProperties(mPhysicalDevices.at(deviceHandle));
    mSelectedPhysicalDeviceQueueFamilyProperties = _getPhysicalDeviceQueueFamilyProperties(mPhysicalDevices.at(deviceHandle));
    mSelectedPhysicalDeviceFeatures = _getPhysicalDeviceFeatures(mPhysicalDevices.at(deviceHandle));
    mSelectedQueueFamilyProperties = mSelectedPhysicalDeviceQueueFamilyProperties.at(queueFamilyHandle);
    return true;
}

const VkPhysicalDeviceProperties&
VkDeviceHolder::_getPhysicalDeviceProperties(const VkPhysicalDevice& device)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

#ifdef DEBUG
    _dumpPhysicalDeviceProperties(properties);
#endif
    return properties;
}

const VkPhysicalDeviceFeatures&
VkDeviceHolder::_getPhysicalDeviceFeatures(const VkPhysicalDevice& device)
{
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);

#ifdef DEBUG
    _dumpPhysicalDeviceFeatures(features);
#endif
    return features;
}

const std::vector<VkQueueFamilyProperties>&
VkDeviceHolder::_getPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice& device)
{
    std::vector<VkQueueFamilyProperties> properties;
    uint32_t familyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);
    properties.resize(familyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, properties.data());

#ifdef DEBUG
    for(const auto& family : properties)
    {
        _dumpPhysicalDeviceQueueFamilyProperties(family);
    }
#endif

    return properties;
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
