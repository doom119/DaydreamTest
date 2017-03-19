//
// Created by Doom119 on 17/2/28.
//

#include <vector>
#include "Vulkan.h"

bool Vulkan::init()
{
    pInstanceHolder = new VkInstanceHolder();
    const VkInstance& vkInstance = pInstanceHolder->createInstance("VulkanTest", 1, "VulkanTest", 1);
    pDeviceHolder = new VkDeviceHolder(vkInstance);

    return true;
}

void Vulkan::shutdown()
{
    delete pInstanceHolder;
    delete pDeviceHolder;
}

bool Vulkan::setSurface(ANativeWindow *window)
{
    if(nullptr == pInstanceHolder || nullptr == pDeviceHolder || nullptr == window)
    {
        return false;
    }

    if(nullptr != pSurfaceHolder)
    {
        delete pSurfaceHolder;
    }

    pSurfaceHolder = new VkSurfaceHolder(pInstanceHolder, pDeviceHolder);
    pSurfaceHolder->createAndroidSurface(window);

    int deviceCount = pDeviceHolder->getDeviceCount();
    int selectedDevice = -1;
    int selectedGraphicsQueueFamily = -1;
    int selectedPresentQueueFamily = -1;
    for(int i = 0; i < deviceCount; ++i)
    {
        selectedGraphicsQueueFamily = pDeviceHolder->supportGraphicsQueueFamily(i);
        selectedPresentQueueFamily = pDeviceHolder->supportPresentQueueFamily(i, pSurfaceHolder->getSurface());
        if(selectedGraphicsQueueFamily >=0
                && selectedPresentQueueFamily >=0
                && pDeviceHolder->supportSwapChain(i))
        {
            selectedDevice = i;
        }
    }
    LOGD("selectedDevice=%d, selectedGraphicsQueueFamily=%d", selectedDevice, selectedGraphicsQueueFamily);
    if(selectedDevice < 0)
    {
        return false;
    }

    pDeviceHolder->selectPhysicalDevice(selectedDevice);
    pDeviceHolder->selectGraphicsQueueFamily(selectedGraphicsQueueFamily);
    pDeviceHolder->selectPresentQueueFamily(selectedPresentQueueFamily);
    pDeviceHolder->addExtensionName(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    pDeviceHolder->createLogicalDevice();
    return true;

}


