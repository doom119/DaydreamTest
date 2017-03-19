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
    VkSurfaceHolder(const VkInstanceHolder*, const VkDeviceHolder*);

    bool createAndroidSurface(ANativeWindow *window);
    const VkSurfaceKHR& getSurface();
    bool supportSwapChain();
    bool selectSurfaceFormat(VkFormat format);
    bool selectSurfaceColorSpace(VkColorSpaceKHR colorSpace);
    bool selectSurfacePresentMode(VkPresentModeKHR present);

    virtual ~VkSurfaceHolder() { vkDestroySurfaceKHR(pInstanceHolder->mInstance, mSurface, nullptr); }

private:
    bool _getSurfaceCapalities(VkSurfaceCapabilitiesKHR &capabilities);
    bool _getSurfaceFormats(std::vector<VkSurfaceFormatKHR> &formats);
    bool _getSurfacePresentModes(std::vector<VkPresentModeKHR> &presents);

    void _dumpSurfaceCapabilities(const VkSurfaceCapabilitiesKHR& capabilities);
    void _dumpSurfaceFormat(const VkSurfaceFormatKHR& format);
    void _dumpSurfacePresentMode(const VkPresentModeKHR& present);

private:
    const VkInstanceHolder* pInstanceHolder;
    const VkDeviceHolder* pDeviceHolder;

    bool mHasSurface;
    VkFormat mSelectedFormat;
    VkColorSpaceKHR mSelectedColorSpace;
    VkPresentModeKHR mSelectedPresentMode;
    VkSurfaceKHR mSurface;
    VkSurfaceCapabilitiesKHR mCapabilities;
    std::vector<VkSurfaceFormatKHR> mFormats;
    std::vector<VkPresentModeKHR> mPresentModes;
};


#endif //DAYDREAMTEST_VKSURFACEHOLDER_H
