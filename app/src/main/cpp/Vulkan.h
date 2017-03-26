//
// Created by Doom119 on 17/2/28.
//

#ifndef DAYDREAMTEST_VULKAN_H
#define DAYDREAMTEST_VULKAN_H

#include <vulkan/vulkan.h>
#include <android/native_window.h>
#include <android/asset_manager.h>
#include "log.h"
#include "VkInstanceHolder.h"
#include "VkDeviceHolder.h"
#include "VkSurfaceHolder.h"
#include "VkPipelineHolder.h"

class Vulkan
{
private:
    VkInstanceHolder* pInstanceHolder;
    VkDeviceHolder* pDeviceHolder;
    VkSurfaceHolder* pSurfaceHolder;
    VkPipelineHolder* pPipeline;
    AAssetManager* pAssetManager;

public:
    Vulkan():pInstanceHolder(nullptr), pDeviceHolder(nullptr), pSurfaceHolder(nullptr){}
    bool createInstance(AAssetManager* pAssets);

    //must call createInstance first
    bool createAndroidSurface(ANativeWindow *window);

    //must call createAndroidSurface first
    bool createDevice();
    void shutdown();
};


#endif //DAYDREAMTEST_VULKAN_H
