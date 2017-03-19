//
// Created by Doom119 on 17/3/9.
//

#include "VkInstanceHolder.h"
#include "log.h"

VkInstanceHolder::VkInstanceHolder() {
    _getExtensionProperties();

    _getLayerProperties();
}

void VkInstanceHolder::addExtensionName(const char* name)
{
    if(nullptr == name)
        return;

    mExtensionNames.push_back(name);
}

bool VkInstanceHolder::supportAndroidSurface()
{
    for (auto &&property : mExtProperties)
    {
        if(!strcmp(property.extensionName, VK_KHR_ANDROID_SURFACE_EXTENSION_NAME))
        {
            return true;
        }
    }

    return false;
}

void VkInstanceHolder::release()
{
    mExtProperties.clear();
    mLayerProperties.clear();
    vkDestroyInstance(mInstance, nullptr);
}

const VkInstance& VkInstanceHolder::getInstance() const
{
    return mInstance;
}

void VkInstanceHolder::_getLayerProperties()
{
    uint32_t propertyCount = 0;
    VkResult result;
    result = vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGW("Enumerate Instance Layouer Properties Count Failed, result=%d", result);
    }
    else
    {
        mLayerProperties.resize(propertyCount);
        result = vkEnumerateInstanceLayerProperties(&propertyCount, mLayerProperties.data());
        if(VK_SUCCESS != result)
        {
            LOGW("Get Instance Layer Properties Failed, result=%d", result);
            return;
        }

#ifdef DEBUG
            _dumpLayerProperties();
#endif
    }
}

void VkInstanceHolder::_getExtensionProperties()
{
    uint32_t propertyCount = 0;
    VkResult result;
    result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
    if(result != VK_SUCCESS)
    {
        LOGW("Enumerate Instance Extension Properties Failed, result=%d", result);
    }
    else
    {
        mExtProperties.resize(propertyCount);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, mExtProperties.data());
        if(VK_SUCCESS != result)
        {
            LOGW("Get Instance Extension Properties Failed, result=%d", result);
            return;
        }

#ifdef DEBUG
        _dumpExtensionProperties();
#endif
    }
}

bool VkInstanceHolder::createInstance(std::string appName, uint32_t appVersion, std::string engineName, uint32_t engineVersion)
{
    VkApplicationInfo appInfo = {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pNext = nullptr,
            .pApplicationName = appName.c_str(),
            .applicationVersion = appVersion,
            .pEngineName = engineName.c_str(),
            .engineVersion = engineVersion,
            .apiVersion = VK_API_VERSION_1_0,
    };

    VkInstanceCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = static_cast<uint32_t>(mExtensionNames.size()),
            .ppEnabledExtensionNames = mExtensionNames.data(),
            .enabledLayerCount = 0,
            .ppEnabledLayerNames = nullptr
    };

    VkResult result = vkCreateInstance(&createInfo, nullptr, &mInstance);
    if(VK_SUCCESS != result)
    {
        LOGE("Create Vulkan Instance Failed, result=%d", result);
        return false;
    }

    LOGI("Create Vulkan Instance Success");
    return true;
}

void VkInstanceHolder::_dumpExtensionProperties()
{
    LOGD("Instance Extension Properties:\n");
    for(const auto& property : mExtProperties)
    {
        LOGD("\textensionName: %s", property.extensionName);
        LOGD("\tspecVersion: %d", property.specVersion);
    }
}

void VkInstanceHolder::_dumpLayerProperties()
{
    LOGD("Instance Layer Properties:\n");
    for(const auto& property : mLayerProperties)
    {
        LOGD("\tlayerName", property.layerName);
        LOGD("\tdescription: %s", property.description);
        LOGD("\tspecVersion: %d", property.specVersion);
        LOGD("\timplementationVersion: %d", property.implementationVersion);
    }
}