#include <jni.h>
#include <string>
#include <vulkan/vulkan.h>
#include <vector>
#include "log.h"

extern "C"
jstring
Java_com_doom119_test_daydreamtest_Vulkan_stringFromJNI(
        JNIEnv *env,
        jobject /* this */)
{
    LOGI("stringFromJNI");
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
    VkInstance instance;
    if(vkCreateInstance(&intanceCreateInfo, nullptr, &instance) != VK_SUCCESS)
    {
        LOGE("Create Vulkan Instance Failed!!!");
    }
    else
    {
        LOGI("Create Vulkan Instance Success!!!");
    }

    LOGI("Vulkan Physical Device Properties:\n");
    uint32_t gpuCount = 0;
    vkEnumeratePhysicalDevices(instance, &gpuCount, nullptr);
    LOGI("\tgpuCount=%d", gpuCount);
    VkPhysicalDevice devices[gpuCount];
    vkEnumeratePhysicalDevices(instance, &gpuCount, devices);
    VkPhysicalDevice selectedDevice = devices[0];
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(selectedDevice, &properties);
    LOGI("\tDevice Name:%s", properties.deviceName);
    LOGI("\tDevice apiVersion:%d.%d.%d",
         VK_VERSION_MAJOR(properties.apiVersion),
         VK_VERSION_MINOR(properties.apiVersion),
         VK_VERSION_PATCH(properties.apiVersion));
    LOGI("\tmaxImageDimension2D=%d", properties.limits.maxImageDimension2D);
    LOGI("\tmaxImageDimension3D=%d", properties.limits.maxImageDimension3D);

    LOGI("Vulkan Queue Family Properties:\n");
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(selectedDevice, &queueFamilyCount, nullptr);
    LOGI("\tQueue Family Count=%d", queueFamilyCount);
    VkQueueFamilyProperties queueFamilyProperties[queueFamilyCount];
    vkGetPhysicalDeviceQueueFamilyProperties(selectedDevice, &queueFamilyCount, queueFamilyProperties);
    LOGI("\tQueue Count=%d", queueFamilyProperties[0].queueCount);
    LOGI("\tQueue Flags=%x", queueFamilyProperties[0].queueFlags);

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
    VkDevice device;
    vkCreateDevice(selectedDevice, &deviceCreateInfo, nullptr, &device);

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
