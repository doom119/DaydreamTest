//
// Created by Doom119 on 17/3/16.
//

#include "VkSurfaceHolder.h"
#include "log.h"

VkSurfaceHolder::VkSurfaceHolder(
        const VkInstanceHolder* instanceHolder,
        const VkDeviceHolder* deviceHolder)
{
    pInstanceHolder = instanceHolder;
    pDeviceHolder = deviceHolder;
    mHasSurface = false;
    mSelectedFormat = VkFormat::VK_FORMAT_UNDEFINED;
    mSelectedColorSpace = VkColorSpaceKHR::VK_COLOR_SPACE_MAX_ENUM_KHR;
    mSelectedPresentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
}

bool VkSurfaceHolder::createAndroidSurface(ANativeWindow *window)
{
    if(nullptr == pInstanceHolder || nullptr == pDeviceHolder)
    {
        return false;
    }

    VkAndroidSurfaceCreateInfoKHR createInfo = {
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .flags = 0,
            .pNext = nullptr,
            .window = window
    };
    VkResult result = vkCreateAndroidSurfaceKHR(pInstanceHolder->mInstance, &createInfo,
                                                nullptr, &mSurface);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Android Surface Failed, result=%d", result);
        return false;
    }

    mHasSurface = true;

//    _getSurfaceCapalities(mCapabilities);
//    _getSurfaceFormats(mFormats);
//    _getSurfacePresentModes(mPresentModes);

    return true;
}

const VkSurfaceKHR& VkSurfaceHolder::getSurface()
{
    return mSurface;
}

bool VkSurfaceHolder::supportSwapChain()
{
    if(mFormats.empty() || mPresentModes.empty())
    {
        return false;
    }

    return true;
}

bool VkSurfaceHolder::selectSurfaceFormat(VkFormat format)
{
    if(1 == mFormats.size() && mFormats.at(0).format == VkFormat::VK_FORMAT_UNDEFINED)
    {
        mSelectedFormat = format;
        return true;
    }

    for (auto &&fmt : mFormats)
    {
        if(fmt.format == format)
        {
            mSelectedFormat = format;
            return true;
        }
    }

    return false;
}

bool VkSurfaceHolder::selectSurfaceColorSpace(VkColorSpaceKHR colorSpace)
{
    if(1 == mFormats.size() && mFormats.at(0).format == VkFormat::VK_FORMAT_UNDEFINED)
    {
        mSelectedColorSpace = colorSpace;
        return true;
    }

    for (auto &&fmt : mFormats)
    {
        if(fmt.colorSpace == colorSpace)
        {
            mSelectedColorSpace = colorSpace;
            return true;
        }
    }

    return false;
}

bool VkSurfaceHolder::selectSurfacePresentMode(VkPresentModeKHR present)
{
    for (auto &&mode : mPresentModes)
    {
        if(mode == present)
        {
            mSelectedPresentMode = present;
            return true;
        }
    }

    return false;
}

bool VkSurfaceHolder::_getSurfaceCapalities(VkSurfaceCapabilitiesKHR &capabilities)
{
    if(!mHasSurface)
    {
        LOGW("Surface has not been created yet");
        return false;
    }

    if(pDeviceHolder->mSelectedPhysicalDevice == nullptr)
    {
        LOGW("Physical Device has not selected yet");
        return false;
    }

    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            pDeviceHolder->mSelectedPhysicalDevice, mSurface, &capabilities);
    if(VK_SUCCESS != result)
    {
        LOGW("Get Surface Capabilities Failed, result=%d", result);
        return false;
    }

#ifdef DEBUG
    _dumpSurfaceCapabilities(capabilities);
#endif
    return true;
}
bool VkSurfaceHolder::_getSurfaceFormats(std::vector<VkSurfaceFormatKHR> &formats)
{
    if(!mHasSurface)
    {
        LOGW("Surface has not been created yet");
        return false;
    }

    if(pDeviceHolder->mSelectedPhysicalDevice == nullptr)
    {
        LOGW("Physical Device has not selected yet");
        return false;
    }

    uint32_t count = 0;
    VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(
            pDeviceHolder->mSelectedPhysicalDevice, mSurface, &count, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGW("Get Surface Format Count Failed, result=%d", result);
        return false;
    }

    formats.resize(count);
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(
            pDeviceHolder->mSelectedPhysicalDevice, mSurface, &count, formats.data());
    if(VK_SUCCESS != result)
    {
        LOGW("Get Surface Format Failed, result=%d", result);
        return false;
    }

#ifdef DEBUG
    for(const auto& format : formats)
    _dumpSurfaceFormat(format);
#endif

    return true;
}
bool VkSurfaceHolder::_getSurfacePresentModes(std::vector<VkPresentModeKHR> &presents)
{
    if(!mHasSurface)
    {
        LOGW("Surface has not been created yet");
        return false;
    }

    if(pDeviceHolder->mSelectedPhysicalDevice == nullptr)
    {
        LOGW("Physical Device has not selected yet");
        return false;
    }

    uint32_t count = 0;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(
            pDeviceHolder->mSelectedPhysicalDevice, mSurface, &count, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGW("Get Surface Present Mode Count Failed, result=%d", result);
        return false;
    }

    result = vkGetPhysicalDeviceSurfacePresentModesKHR(
            pDeviceHolder->mSelectedPhysicalDevice, mSurface, &count, presents.data());
    if(VK_SUCCESS != result)
    {
        LOGW("Get Surface Present Modes Failed, result=%d", result);
        return false;
    }

#ifdef DEBUG
    for (auto &&present : presents)
    {
        _dumpSurfacePresentMode(present);
    }
#endif

    return true;

}

void VkSurfaceHolder::_dumpSurfaceCapabilities(const VkSurfaceCapabilitiesKHR &capabilities)
{
    LOGD("Surface Capalities:\n");
    LOGD("\tcurrentExtent.width: %d", capabilities.currentExtent.width);
    LOGD("\tcurrentExtent.height: %d", capabilities.currentExtent.height);
    LOGD("\tmaxImageCount= %d", capabilities.maxImageCount);
    LOGD("\tmaxImageExtent.width: %d", capabilities.maxImageExtent.width);
    LOGD("\tmaxImageExtent.height: %d", capabilities.maxImageExtent.height);
    LOGD("\tminImageCount: %d", capabilities.minImageCount);
    LOGD("\tminImageExtent.width: %d", capabilities.minImageExtent.width);
    LOGD("\tminImageExtent.height: %d", capabilities.minImageExtent.height);
    LOGD("\tcurrentTransform: %d", capabilities.currentTransform);
    LOGD("\tmaxImageArrayLayers: %d", capabilities.maxImageArrayLayers);
    LOGD("\tsupportedCompositeAlpha: %d", capabilities.supportedCompositeAlpha);
    LOGD("\tsupportedTransforms: %d", capabilities.supportedTransforms);
    LOGD("\tsupportedUsageFlags: %d", capabilities.supportedUsageFlags);
}

void VkSurfaceHolder::_dumpSurfaceFormat(const VkSurfaceFormatKHR& format)
{
    LOGD("Surface Format:\n");
    switch(format.colorSpace)
    {
        case VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR:
            LOGD("\tcolorSpace: %d, %s", format.colorSpace, "SRGB_NONLINEAR");
            break;
        case VkColorSpaceKHR::VK_COLOR_SPACE_RANGE_SIZE_KHR:
            LOGD("\tcolorSpace: %d, %s", format.colorSpace, "RANGE_SIZE");
            break;
        case VkColorSpaceKHR::VK_COLOR_SPACE_MAX_ENUM_KHR:
            LOGD("\tcolorSpace: %d, %s", format.colorSpace, "MAX_ENUM");
            break;
        default:
            LOGW("\tcolorSpace not defined: %d", format.colorSpace);
    }
    LOGD("\tformat: %d", format.format);
}

void VkSurfaceHolder::_dumpSurfacePresentMode(const VkPresentModeKHR& present)
{
    LOGD("Physical Device Surface Present Mode:\n");
    switch(present)
    {
        case VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR:
            LOGD("\tpresent mode: %d, %s", present, "IMMEDIATE");
            break;
        case VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR:
            LOGD("\tpresent mode: %d, %s", present, "MAILBOX");
            break;
        case VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR:
            LOGD("\tpresent mode: %d, %s", present, "FIFO");
            break;
        case VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR:
            LOGD("\tpresent mode: %d, %s", present, "FIFO_RELAXED");
            break;
        case VkPresentModeKHR::VK_PRESENT_MODE_RANGE_SIZE_KHR:
            LOGD("\tpresent mode: %d, %s", present, "RANGE_SIZE");
            break;
        case VkPresentModeKHR::VK_PRESENT_MODE_MAX_ENUM_KHR:
            LOGD("\tpresent mode: %d, %s", present, "MAX_ENUM");
            break;
        default:
            LOGW("\tpresent mode not defined: %d", present);
    }
}