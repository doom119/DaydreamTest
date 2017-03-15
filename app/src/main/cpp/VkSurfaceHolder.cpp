//
// Created by Doom119 on 17/3/16.
//

#include "VkSurfaceHolder.h"
#include "log.h"

VkSurfaceHolder::VkSurfaceHolder(const VkInstanceHolder* instanceHolder, const VkDeviceHolder* deviceHolder)
{
    pInstanceHolder = instanceHolder;
    pDeviceHolder = deviceHolder;
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

    return true;
}

const VkSurfaceKHR& VkSurfaceHolder::getSurface()
{
    return mSurface;
}
