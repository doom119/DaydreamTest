//
// Created by Doom119 on 17/3/16.
//

#include "VkSurfaceHolder.h"
#include "log.h"

VkSurfaceHolder::VkSurfaceHolder()
{
    mHasSurface = false;
    mSelectedFormat = VkFormat::VK_FORMAT_UNDEFINED;
    mSelectedColorSpace = VkColorSpaceKHR::VK_COLOR_SPACE_MAX_ENUM_KHR;
    mSelectedPresentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
}

bool VkSurfaceHolder::createAndroidSurface(const VkInstance& instance, ANativeWindow *window)
{
    if(nullptr == instance || nullptr == window)
    {
        return false;
    }

    VkAndroidSurfaceCreateInfoKHR createInfo = {
            .sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR,
            .flags = 0,
            .pNext = nullptr,
            .window = window
    };
    VkResult result = vkCreateAndroidSurfaceKHR(instance, &createInfo,
                                                nullptr, &mSurface);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Android Surface Failed, result=%d", result);
        mHasSurface = false;
        return false;
    }

    LOGI("Create Android Surface Success");
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

bool VkSurfaceHolder::selectSurfaceFormat(const VkPhysicalDevice& device, VkFormat format)
{
    if(mFormats.empty())
    {
        _getSurfaceCapalities(device, mCapabilities);
        _getSurfaceFormats(device, mFormats);
    }

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

    LOGW("Surface not supported the specified VkFormat: %d", format);
    return false;
}

bool VkSurfaceHolder::selectSurfaceColorSpace(const VkPhysicalDevice& device, VkColorSpaceKHR colorSpace)
{
    if(mFormats.empty())
    {
        _getSurfaceFormats(device, mFormats);
    }

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

    LOGW("Surface not supported the specified VkColorSpaceKHR: %d", colorSpace);
    return false;
}

bool VkSurfaceHolder::selectSurfacePresentMode(const VkPhysicalDevice& device, VkPresentModeKHR present)
{
    if(mPresentModes.empty())
    {
        _getSurfacePresentModes(device, mPresentModes);
    }

    for (auto &&mode : mPresentModes)
    {
        if(mode == present)
        {
            mSelectedPresentMode = present;
            return true;
        }
    }

    LOGW("Surface not supported the specified VkPresentModeKHR: %d", present);
    return false;
}

bool VkSurfaceHolder::createSwapChain(const VkDevice& device, uint32_t graphicsQueueFamily, uint32_t presentQueueFamily)
{
    uint32_t queueFamilyCount = graphicsQueueFamily == presentQueueFamily ? 1 : 2;
    std::vector<uint32_t> queueFamilyIndices;
    queueFamilyIndices.push_back(graphicsQueueFamily);
    if(queueFamilyCount == 2)
    {
        queueFamilyIndices.push_back(presentQueueFamily);
    }

    VkSwapchainCreateInfoKHR info = {
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .pNext = nullptr,
            .flags = 0,
            .imageColorSpace = mSelectedColorSpace,
            .imageFormat = mSelectedFormat,
            .presentMode = mSelectedPresentMode,
            .imageExtent = mCapabilities.currentExtent,
            .minImageCount = mCapabilities.minImageCount,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .preTransform = mCapabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .clipped = VK_TRUE,
            .oldSwapchain = nullptr,
            .queueFamilyIndexCount = queueFamilyCount,
            .pQueueFamilyIndices = queueFamilyIndices.data(),
            .surface = mSurface
    };

    VkResult result = vkCreateSwapchainKHR(device, &info, nullptr, &mSwapChain);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Swapchain Failed, result=%d", result);
        return false;
    }

    LOGI("Create Swapchain Sucess");
    mSurfaceWidth = mCapabilities.currentExtent.width;
    mSurfaceHeight = mCapabilities.currentExtent.height;

    uint32_t imageCount = 0;
    result = vkGetSwapchainImagesKHR(device, mSwapChain, &imageCount, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGW("Get SwapChain Images Count Failed, result=%d", result);
        return false;
    }
    LOGI("Get SwapChain Images Count Success, count=%d", imageCount);
    mImages.resize(imageCount);
    result = vkGetSwapchainImagesKHR(device, mSwapChain, &imageCount, mImages.data());
    if(VK_SUCCESS != result)
    {
        LOGW("Get SwapChain Images Failed, result=%d", result);
        return false;
    }
    LOGI("Get SwapChain Images Success");

    for (auto &&image : mImages)
    {
        VkImageViewCreateInfo info = {
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .format = mSelectedFormat,
                .image = image,
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .components = {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
                .subresourceRange = {
                        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                        .baseArrayLayer = 0,
                        .baseMipLevel = 0,
                        .levelCount = 1,
                        .layerCount = 1
                },
        };
        VkImageView* imageView = new VkImageView;
        VkResult result = vkCreateImageView(device, &info, nullptr, imageView);
        if(VK_SUCCESS != result)
        {
            LOGW("Create ImageView Failed, result=%d", result);
            return false;
        };
        LOGI("Create ImageView Success");
        mImageViews.push_back(imageView);
    }
    return true;
}

const std::vector<VkImageView*>& VkSurfaceHolder::getImageViews() const
{
    return mImageViews;
}

uint32_t VkSurfaceHolder::getWidth() const
{
    return mSurfaceWidth;
}

uint32_t VkSurfaceHolder::getHeight() const
{
    return mSurfaceHeight;
}

VkFormat VkSurfaceHolder::getFormat() const
{
    return mSelectedFormat;
}

VkSwapchainKHR & VkSurfaceHolder::getSwapChain()
{
    return mSwapChain;
}

bool VkSurfaceHolder::_getSurfaceCapalities(const VkPhysicalDevice& device, VkSurfaceCapabilitiesKHR &capabilities)
{
    if(!mHasSurface)
    {
        LOGW("Surface has not been created yet");
        return false;
    }

    if(nullptr == device)
    {
        LOGW("device is null");
        return false;
    }

    VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
            device, mSurface, &capabilities);
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
bool VkSurfaceHolder::_getSurfaceFormats(const VkPhysicalDevice& device, std::vector<VkSurfaceFormatKHR> &formats)
{
    if(!mHasSurface)
    {
        LOGW("Surface has not been created yet");
        return false;
    }

    if(nullptr == device)
    {
        LOGW("device is null");
        return false;
    }

    uint32_t count = 0;
    VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, mSurface, &count, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGW("Get Surface Format Count Failed, result=%d", result);
        return false;
    }

    formats.resize(count);
    result = vkGetPhysicalDeviceSurfaceFormatsKHR(
            device, mSurface, &count, formats.data());
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
bool VkSurfaceHolder::_getSurfacePresentModes(const VkPhysicalDevice& device, std::vector<VkPresentModeKHR> &presents)
{
    if(!mHasSurface)
    {
        LOGW("Surface has not been created yet");
        return false;
    }

    if(nullptr == device)
    {
        LOGW("Physical Device has not selected yet");
        return false;
    }

    uint32_t count = 0;
    VkResult result = vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, mSurface, &count, nullptr);
    if(VK_SUCCESS != result)
    {
        LOGW("Get Surface Present Mode Count Failed, result=%d", result);
        return false;
    }

    presents.resize(count);
    result = vkGetPhysicalDeviceSurfacePresentModesKHR(
            device, mSurface, &count, presents.data());
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

void VkSurfaceHolder::release(const VkInstance& instance, const VkDevice& device)
{
    for (int i = 0; i < mImageViews.size(); ++i)
    {
        vkDestroyImageView(device, *mImageViews[i], nullptr);
        delete mImageViews[i];
    }
    for (auto &&image : mImages)
    {
        vkDestroyImage(device, image, nullptr);
    }
    vkDestroySurfaceKHR(instance, mSurface, nullptr);
    vkDestroySwapchainKHR(device, mSwapChain, nullptr);
}