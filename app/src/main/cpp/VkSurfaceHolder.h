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

private:
    bool _getPhysicalDeviceSurfaceCapalities();
    bool _getPhysicalDeviceSurfaceFormats();
    bool _getPhysicalDeviceSurfacePresentModes();

private:
    const VkInstanceHolder* pInstanceHolder;
    const VkDeviceHolder* pDeviceHolder;

    VkSurfaceKHR mSurface;
};


#endif //DAYDREAMTEST_VKSURFACEHOLDER_H
