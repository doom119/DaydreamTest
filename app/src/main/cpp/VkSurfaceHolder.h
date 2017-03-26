//
// Created by Doom119 on 17/3/16.
//

#ifndef DAYDREAMTEST_VKSURFACEHOLDER_H
#define DAYDREAMTEST_VKSURFACEHOLDER_H


#include "VkInstanceHolder.h"
#include "VkDeviceHolder.h"

class VkSurfaceHolder
{
public:
    VkSurfaceHolder();

    bool createAndroidSurface(const VkInstance& instance, ANativeWindow *window);
    const VkSurfaceKHR& getSurface();
    bool selectSurfaceFormat(const VkPhysicalDevice& device, VkFormat format);
    bool selectSurfaceColorSpace(const VkPhysicalDevice& device, VkColorSpaceKHR colorSpace);
    bool selectSurfacePresentMode(const VkPhysicalDevice& device, VkPresentModeKHR present);

    bool createSwapChain(const VkDevice& device, uint32_t graphicsQueueFamily, uint32_t presentQueueFamily);
    uint32_t getWidth() const;
    uint32_t getHeight() const;
    VkFormat getFormat() const;
    void release(const VkInstance& instance, const VkDevice& device);
private:
    bool _getSurfaceCapalities(const VkPhysicalDevice& device, VkSurfaceCapabilitiesKHR &capabilities);
    bool _getSurfaceFormats(const VkPhysicalDevice& device, std::vector<VkSurfaceFormatKHR> &formats);
    bool _getSurfacePresentModes(const VkPhysicalDevice& device, std::vector<VkPresentModeKHR> &presents);

    void _dumpSurfaceCapabilities(const VkSurfaceCapabilitiesKHR& capabilities);
    void _dumpSurfaceFormat(const VkSurfaceFormatKHR& format);
    void _dumpSurfacePresentMode(const VkPresentModeKHR& present);

private:

    bool mHasSurface;
    VkFormat mSelectedFormat;
    VkColorSpaceKHR mSelectedColorSpace;
    VkPresentModeKHR mSelectedPresentMode;
    VkSurfaceKHR mSurface;
    VkSurfaceCapabilitiesKHR mCapabilities;
    std::vector<VkSurfaceFormatKHR> mFormats;
    std::vector<VkPresentModeKHR> mPresentModes;
    uint32_t mSurfaceWidth;
    uint32_t mSurfaceHeight;

    VkSwapchainKHR mSwapChain;
};


#endif //DAYDREAMTEST_VKSURFACEHOLDER_H
