//
// Created by Doom119 on 17/3/9.
//

#ifndef DAYDREAMTEST_VKINSTANCEHELPER_H
#define DAYDREAMTEST_VKINSTANCEHELPER_H


#include <vulkan/vulkan.h>
#include <string>
#include <vector>

class VkInstanceHolder
{
public:
    friend class VkDeviceHolder;
    friend class VkSurfaceHolder;

    VkInstanceHolder();
    void addExtensionName(const char* name);
    bool supportAndroidSurface();
    bool createInstance(std::string appName, uint32_t appVersion, std::string engineName, uint32_t engineVersion);
    const VkInstance& getInstance() const;
    void release();
private:
    VkInstance mInstance;
    std::vector<const char*> mExtensionNames;
    std::vector<VkExtensionProperties> mExtProperties;
    std::vector<VkLayerProperties> mLayerProperties;

private:
    void _getExtensionProperties();
    void _getLayerProperties();

    void _dumpExtensionProperties();
    void _dumpLayerProperties();
};


#endif //DAYDREAMTEST_VKINSTANCEHELPER_H
