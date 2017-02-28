//
// Created by Doom119 on 17/2/28.
//

#include <vector>
#include "Vulkan.h"

bool Vulkan::init()
{
    return _createInstance() && _selectPhysicalDevice() && _createLogicDevice();
}

bool Vulkan::_createInstance()
{
    _dumpInstanceExtensionProperties();

    VkApplicationInfo appinfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = "DaydreamTest",
            .pEngineName = "VulkanTest",
            .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
            .apiVersion = VK_API_VERSION_1_0,
            .engineVersion = VK_MAKE_VERSION(1, 0, 0)
    };

    std::vector<const char *> instanceExt;
    instanceExt.push_back("VK_KHR_surface");
    instanceExt.push_back("VK_KHR_android_surface");
    VkInstanceCreateInfo intanceCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &appinfo,
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr,
            .enabledExtensionCount = static_cast<uint32_t>(instanceExt.size()),
            .ppEnabledExtensionNames = instanceExt.data()
    };

    VkResult result = vkCreateInstance(&intanceCreateInfo, nullptr, &mVkInstance);
    if(result == VK_SUCCESS)
    {
        LOGI("Create Vulkan Instance Success");
        return true;
    }
    else
    {
        LOGE("Create Vulkan Instance Failed, result=%d", result);
        return false;
    }
}

bool Vulkan::_selectPhysicalDevice()
{
    uint32_t gpuCount = 0;
    VkResult result = vkEnumeratePhysicalDevices(mVkInstance, &gpuCount, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGE("Enumerate Physical Devices Failed, result=%d", result);
        return false;
    }
    VkPhysicalDevice devices[gpuCount];
    result = vkEnumeratePhysicalDevices(mVkInstance, &gpuCount, devices);
    if(VK_SUCCESS != result)
    {
        LOGE("Get Physical Devices Failed, result=%d", result);
        return false;
    }
    mVkPhysicalDevice = devices[0];
    _dumpPhysicalDeviceProperties();
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

    LOGD("Create Android Surface Success");
    return true;
}

void Vulkan::_dumpInstanceExtensionProperties()
{
    uint32_t propertyCount = 0;
    VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGW("Enumerate Instance Extension Properties Failed, result=%d", result);
        return;
    }
    VkExtensionProperties properties[propertyCount];
    result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties);
    if(VK_SUCCESS != result)
    {
        LOGW("Get Instance Extension Properties Failed, result=%d", result);
        return;
    }
    for(int i = 0; i < propertyCount; ++i)
    {
        LOGD("Extension Property %d:\n", i);
        LOGD("\textensionName: %s", properties[i].extensionName);
        LOGD("\tspecVersion: %d", properties[i].specVersion);
    }
}

void Vulkan::_dumpPhysicalDeviceProperties()
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(mVkPhysicalDevice, &properties);
    LOGD("Vulkan Physical Device Properties:\n");
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

