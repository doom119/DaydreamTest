//
// Created by Doom119 on 17/2/28.
//

#include <vector>
#include "Vulkan.h"

bool Vulkan::createInstance(AAssetManager* pAssets)
{
    pAssetManager = pAssets;
    pInstanceHolder = new VkInstanceHolder();
    if(!pInstanceHolder->supportAndroidSurface())
    {
        return false;
    }
    pInstanceHolder->addExtensionName(VK_KHR_SURFACE_EXTENSION_NAME);
    pInstanceHolder->addExtensionName(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
    pInstanceHolder->createInstance("VulkanTest", 1, "VulkanTest", 1);

    return true;
}

void Vulkan::shutdown()
{
    if(nullptr != pPipeline)
    {
        pPipeline->release(pDeviceHolder->getLogicalDevice());
    }
    delete pPipeline;
    if(nullptr != pSurfaceHolder)
    {
        pSurfaceHolder->release(pInstanceHolder->getInstance(), pDeviceHolder->getLogicalDevice());
    }
    delete pSurfaceHolder;

    if(nullptr != pDeviceHolder)
    {
        pDeviceHolder->release();
    }
    delete pDeviceHolder;

    delete pInstanceHolder;

    delete pAssetManager;
}

//no need VkPhysicalDevice to create VkSurface
bool Vulkan::createAndroidSurface(ANativeWindow *window)
{
    if (nullptr == pInstanceHolder || nullptr == window)
    {
        return false;
    }

    if (nullptr != pSurfaceHolder)
    {
        pSurfaceHolder->release(pInstanceHolder->getInstance(), pDeviceHolder->getLogicalDevice());
        delete pSurfaceHolder;
    }

    pSurfaceHolder = new VkSurfaceHolder();
    return pSurfaceHolder->createAndroidSurface(pInstanceHolder->getInstance(), window);
}

bool Vulkan::createDevice()
{
    if(nullptr == pInstanceHolder || nullptr == pSurfaceHolder)
        return false;

    pDeviceHolder = new VkDeviceHolder(pInstanceHolder->getInstance());
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
    LOGD("selectedDevice=%d, selectedGraphicsQueueFamily=%d, selectedPresentQueueFamily=%d",
         selectedDevice, selectedGraphicsQueueFamily, selectedPresentQueueFamily);
    if(selectedDevice < 0)
    {
        return false;
    }

    pDeviceHolder->selectPhysicalDevice(selectedDevice);
    pDeviceHolder->selectGraphicsQueueFamily(selectedGraphicsQueueFamily);
    pDeviceHolder->selectPresentQueueFamily(selectedPresentQueueFamily);
    pDeviceHolder->addExtensionName(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    pDeviceHolder->createLogicalDevice();

    pSurfaceHolder->selectSurfaceFormat(pDeviceHolder->getSelectedPhysicalDevice(), VkFormat::VK_FORMAT_R8G8B8A8_UNORM);
    pSurfaceHolder->selectSurfaceColorSpace(pDeviceHolder->getSelectedPhysicalDevice(), VkColorSpaceKHR::VK_COLOR_SPACE_SRGB_NONLINEAR_KHR);
    pSurfaceHolder->selectSurfacePresentMode(pDeviceHolder->getSelectedPhysicalDevice(), VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR);
    pSurfaceHolder->createSwapChain(pDeviceHolder->getLogicalDevice(),
                                        pDeviceHolder->getSelectedGraphicsQueueFamily(),
                                        pDeviceHolder->getSelectedPresentQueueFamily());

    pPipeline = new VkPipelineHolder();
    pPipeline->createShader(pDeviceHolder->getLogicalDevice(), pAssetManager);
    pPipeline->createVertexInput();
    pPipeline->createInputAssembly(VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
    pPipeline->createViewPort(0.0f, 0.0f, (float)pSurfaceHolder->getWidth(), (float)pSurfaceHolder->getHeight(), 0.0f, 1.0f);
    pPipeline->createScissor(0, 0, pSurfaceHolder->getWidth(), pSurfaceHolder->getHeight());
    pPipeline->createRasterization(VkPolygonMode::VK_POLYGON_MODE_FILL, 1.0f, VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT, VkFrontFace::VK_FRONT_FACE_CLOCKWISE);
    pPipeline->createMultisampling();
    pPipeline->createColorBlend();
    pPipeline->createLayout(pDeviceHolder->getLogicalDevice());
    pPipeline->createRenderPass(pDeviceHolder->getLogicalDevice(), pSurfaceHolder->getFormat());
    pPipeline->createPipeline(pDeviceHolder->getLogicalDevice());
    pPipeline->createFramebuffer(pDeviceHolder->getLogicalDevice(), pSurfaceHolder->getImageViews(), pSurfaceHolder->getWidth(), pSurfaceHolder->getHeight());
    pPipeline->createCommandPool(pDeviceHolder->getLogicalDevice(), pDeviceHolder->getSelectedGraphicsQueueFamily());
    pPipeline->createCommandBuffer(pDeviceHolder->getLogicalDevice(), pSurfaceHolder->getWidth(), pSurfaceHolder->getHeight());
    return true;
}


