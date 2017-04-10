//
// Created by Doom119 on 21/3/9.
//

#include <memory.h>
#include "VkPipelineHolder.h"
#include "log.h"

VkPipelineHolder::VkPipelineHolder()
{
    mViewportCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    mViewportCreateInfo.flags = 0;
    mViewportCreateInfo.pNext = nullptr;
    mViewportCreateInfo.viewportCount = 0;
    mViewportCreateInfo.pViewports = nullptr;
    mViewportCreateInfo.scissorCount = 0;
    mViewportCreateInfo.pScissors = nullptr;
}

bool VkPipelineHolder::createShader(const VkDevice &device, const AAssetManager* pAssetsManager)
{
    AAsset* vertexFile = AAssetManager_open(const_cast<AAssetManager*>(pAssetsManager),
                                            "shaders/vertex.vert.spv", AASSET_MODE_BUFFER);
    size_t vertexLength = AAsset_getLength(vertexFile);
    char* vertexContent = new char[vertexLength];
    AAsset_read(vertexFile, vertexContent, vertexLength);
    VkShaderModuleCreateInfo vertexInfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .flags = 0,
            .pNext = nullptr,
            .pCode = (uint32_t*)vertexContent,
            .codeSize = vertexLength
    };
    AAsset_close(vertexFile);

    AAsset* fragFile = AAssetManager_open(const_cast<AAssetManager*>(pAssetsManager),
                                          "shaders/fragment.frag.spv", AASSET_MODE_BUFFER);
    size_t fragLength = AAsset_getLength(fragFile);
    char* fragContent = new char[fragLength];
    AAsset_read(fragFile, fragContent, fragLength);
    VkShaderModuleCreateInfo fragInfo = {
            .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
            .flags = 0,
            .pNext = nullptr,
            .pCode = (uint32_t*)fragContent,
            .codeSize = fragLength
    };
    AAsset_close(fragFile);

    VkResult result = vkCreateShaderModule(device, &vertexInfo, nullptr, &mVertexShaderModule);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Vertex Shader Module Failed, result=%d", result);
        return false;
    }
    LOGI("Create Vertex Shader Module Success");

    result = vkCreateShaderModule(device, &fragInfo, nullptr, &mFragmentShaderModule);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Fragment Shader Module Failed, result=%d", result);
        return false;
    }
    LOGI("Create Fragment Shader Module Success");

    VkPipelineShaderStageCreateInfo vertexStage = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = mVertexShaderModule,
            .pName = "main",
            .pSpecializationInfo = nullptr
    };
    VkPipelineShaderStageCreateInfo fragStage = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = mFragmentShaderModule,
            .pName = "main",
            .pSpecializationInfo = nullptr
    };
    mShaderStageCreateInfos = new VkPipelineShaderStageCreateInfo[2]{
            vertexStage, fragStage
    };

//    delete fragContent;
//    delete vertexContent;
    return true;
}

void VkPipelineHolder::createVertexInput()
{
    mVertexInputCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .pVertexAttributeDescriptions = nullptr,
            .vertexAttributeDescriptionCount = 0,
            .vertexBindingDescriptionCount = 0,
            .pVertexBindingDescriptions = nullptr
    };
}

void VkPipelineHolder::createInputAssembly(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable)
{
    mInputAssemblyCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .topology = topology,
            .primitiveRestartEnable = primitiveRestartEnable
    };
}

void VkPipelineHolder::createViewPort(float x, float y, float width, float height, float minDepth, float maxDepth)
{
    VkViewport* viewport = new VkViewport{
        x, y, width, height, minDepth, maxDepth
    };
    mViewportCreateInfo.viewportCount = 1;
    mViewportCreateInfo.pViewports = viewport;
}

void VkPipelineHolder::createScissor(int32_t x, int32_t y, uint32_t width, uint32_t height)
{
    VkRect2D* scissor = new VkRect2D{
            {x, y},
            {width, height}
    };
    mViewportCreateInfo.scissorCount = 1;
    mViewportCreateInfo.pScissors = scissor;
}

void VkPipelineHolder::createRasterization(VkPolygonMode polygonMode, float lineWidth,
                                           VkCullModeFlags cullMode, VkFrontFace frontFace)
{
    mRasterizationCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .depthClampEnable = VK_FALSE,
            .rasterizerDiscardEnable = VK_FALSE,
            .polygonMode = polygonMode,
            .lineWidth = lineWidth,
            .cullMode = cullMode,
            .frontFace = frontFace,
            .depthBiasEnable = VK_FALSE,
            .depthBiasConstantFactor = 0.0f,
            .depthBiasClamp = 0.0f,
            .depthBiasSlopeFactor = 0.0f
    };
}

void VkPipelineHolder::createMultisampling()
{
    mMultisamplingCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .sampleShadingEnable = VK_FALSE,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .minSampleShading = 1.0f,
            .pSampleMask = nullptr,
            .alphaToCoverageEnable = VK_FALSE,
            .alphaToOneEnable = VK_FALSE
    };
}

void VkPipelineHolder::createColorBlend()
{
    VkPipelineColorBlendAttachmentState attachment = {
            .blendEnable = VK_FALSE,
            .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
            .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
            .colorBlendOp = VK_BLEND_OP_ADD,
            .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
            .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
            .alphaBlendOp = VK_BLEND_OP_ADD
    };

    mColorBlendCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .logicOpEnable = VK_FALSE,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = 1,
            .pAttachments = &attachment,
            .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
    };
}

bool VkPipelineHolder::createLayout(const VkDevice& device)
{
    memset(&mLayout, 0, sizeof(mLayout));
    VkPipelineLayoutCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .setLayoutCount = 0,
            .pSetLayouts = nullptr,
            .pushConstantRangeCount = 0,
            .pPushConstantRanges = nullptr
    };
    
    VkResult result = vkCreatePipelineLayout(device, &info, nullptr, &mLayout);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Pipeline Layout Failed, result=%d", result);
        return false;
    }

    LOGI("Create Pipeline Layout Success");
    return true;
}

bool VkPipelineHolder::createRenderPass(const VkDevice& device, VkFormat format)
{
    VkAttachmentDescription colorAttachment = {
            .flags = 0,
            .format = format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };
    VkAttachmentReference colorAttachmentRef = {
            .attachment = 0,
            .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    };
    VkSubpassDescription subpass = {
            .flags = 0,
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &colorAttachmentRef,
            .inputAttachmentCount = 0,
            .pInputAttachments = nullptr,
            .pDepthStencilAttachment = nullptr,
            .preserveAttachmentCount = 0,
            .pPreserveAttachments = nullptr,
            .pResolveAttachments = nullptr
    };
    VkRenderPassCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
            .flags = 0,
            .pNext = nullptr,
            .attachmentCount = 1,
            .pAttachments = &colorAttachment,
            .subpassCount = 1,
            .pSubpasses = &subpass,
            .dependencyCount = 0,
            .pDependencies = nullptr,
    };
    VkResult result = vkCreateRenderPass(device, &info, nullptr, &mRenderPass);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Render Pass Failed, result=%d", result);
        return false;
    }

    LOGI("Create Render Pass Success");
    return true;
}

bool VkPipelineHolder::createPipeline(const VkDevice &device)
{
    memset(&mPipeline, 0, sizeof(mPipeline));
    VkPipelineShaderStageCreateInfo shaderStages[2] = {
            {
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = 0,
                    .stage = VK_SHADER_STAGE_VERTEX_BIT,
                    .module = mVertexShaderModule,
                    .pName = "main",
                    .pSpecializationInfo = nullptr,
            },
            {
                    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                    .pNext = nullptr,
                    .flags = 0,
                    .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                    .module = mFragmentShaderModule,
                    .pName = "main",
                    .pSpecializationInfo = nullptr,
            }
    };
    VkGraphicsPipelineCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .stageCount = 2,
            .pStages = mShaderStageCreateInfos,
            .pVertexInputState = &mVertexInputCreateInfo,
            .pInputAssemblyState = &mInputAssemblyCreateInfo,
            .pViewportState = &mViewportCreateInfo,
            .pRasterizationState = &mRasterizationCreateInfo,
            .pMultisampleState = &mMultisamplingCreateInfo,
            .pDepthStencilState = nullptr,
            .pColorBlendState = &mColorBlendCreateInfo,
            .pDynamicState = nullptr,
            .layout = mLayout,
            .renderPass = mRenderPass,
            .subpass = 0,
            .basePipelineIndex = -1,
            .basePipelineHandle = nullptr
    };
    VkResult result = vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &info, nullptr, &mPipeline);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Graphics Pipeline Failed, result=%d", result);
        return false;
    }
    LOGI("Create Graphics Pipeline Success");
    return true;
}

bool VkPipelineHolder::createFramebuffer(const VkDevice& device, const std::vector<VkImageView*> imageViews, uint32_t width, uint32_t height)
{
    for(int i = 0; i < imageViews.size(); ++i)
    {
        VkFramebufferCreateInfo info = {
                .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .pNext = nullptr,
                .flags = 0,
                .width = width,
                .height = height,
                .layers = 1,
                .attachmentCount = 1,
                .pAttachments = imageViews[i],
                .renderPass = mRenderPass
        };
        VkFramebuffer *buffer = new VkFramebuffer;
        VkResult result = vkCreateFramebuffer(device, &info, nullptr, buffer);
        if(VK_SUCCESS != result)
        {
            LOGW("Create Framebuffer Failed, result=%d", result);
            return false;
        }
        LOGI("Create Framebuffer Success");
        mFramebuffers.push_back(buffer);
    }

    return true;
}

bool VkPipelineHolder::createCommandPool(const VkDevice& device, uint32_t queueFamilyIndex)
{
    VkCommandPoolCreateInfo info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0,
            .queueFamilyIndex = queueFamilyIndex,
    };
    VkResult result = vkCreateCommandPool(device, &info, nullptr, &mCommandPool);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Command Pool Failed, result=%d", result);
        return false;
    }
    LOGI("Create Command Pool Success");
    return true;
}

bool VkPipelineHolder::createCommandBuffer(const VkDevice& device, uint32_t width, uint32_t height)
{
    mCommandBuffers.resize(mFramebuffers.size());
    VkCommandBufferAllocateInfo info = {
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            .pNext = nullptr,
            .commandBufferCount = static_cast<uint32_t>(mFramebuffers.size()),
            .commandPool = mCommandPool,
            .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY
    };
    VkResult result = vkAllocateCommandBuffers(device, &info, mCommandBuffers.data());
    if(VK_SUCCESS != result)
    {
        LOGW("Allocate Command Buffers Failed, result=%d", result);
        return false;
    };
    LOGI("Allocate Command Buffer Success");

    for (int i = 0; i < mCommandBuffers.size(); ++i)
    {
        VkCommandBufferBeginInfo info = {
                .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
                .pNext = nullptr,
                .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
                .pInheritanceInfo = nullptr,
        };
        VkResult result = vkBeginCommandBuffer(mCommandBuffers.at(i), &info);
        if(VK_SUCCESS != result)
        {
            LOGW("Begin Command Buffer Failed, result=%d", result);
            return false;
        }
        LOGI("Begin Command Buffer Success");

        VkClearValue clearColor = {0.0f, 1.0f, 0.0f, 1.0f};
        VkRenderPassBeginInfo renderPassBeginInfo = {
                .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
                .pNext = nullptr,
                .renderPass = mRenderPass,
                .framebuffer = *mFramebuffers.at(i),
                .renderArea.offset = {0, 0},
                .renderArea.extent = {width, height},
                .clearValueCount = 1,
                .pClearValues = &clearColor,
        };
        vkCmdBeginRenderPass(mCommandBuffers.at(i), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(mCommandBuffers.at(i), VK_PIPELINE_BIND_POINT_GRAPHICS, mPipeline);
        vkCmdDraw(mCommandBuffers.at(i), 3, 1, 0, 0);
        vkCmdEndRenderPass(mCommandBuffers.at(i));
        vkEndCommandBuffer(mCommandBuffers.at(i));
    }

    VkFenceCreateInfo fenceInfo = {
            .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
    };
    result = vkCreateFence(device, &fenceInfo, nullptr, &mFence);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Fence Failed, result=%d", result);
        return false;
    }
    LOGW("Create Fence Success");

    VkSemaphoreCreateInfo semInfo = {
            .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            .pNext = nullptr,
            .flags = 0
    };
    result = vkCreateSemaphore(device, &semInfo, nullptr, &mSemaphore);
    if(VK_SUCCESS != result)
    {
        LOGW("Create Semaphore Failed, result=%d", result);
        return false;
    }
    LOGW("Create Semaphore Success");
    return true;
}

void VkPipelineHolder::draw(VkDevice& device, VkSwapchainKHR& swapchain, VkQueue& queue) {
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, mSemaphore, mFence, &imageIndex);
    if(VK_SUCCESS != result) {
        LOGW("Acquire Next Image Failed");
        return;
    }
    LOGI("Acquire Next Image Success");
    result = vkResetFences(device, 1, &mFence);
    if(VK_SUCCESS != result)
    {
        LOGW("Reset Fences Failed, result=%d", result);
        return;
    }
    LOGI("Reset Fences Success");

    VkSubmitInfo submit_info = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .pNext = nullptr,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &mSemaphore,
            .commandBufferCount = 1,
            .pCommandBuffers = &mCommandBuffers.at(imageIndex),
            .signalSemaphoreCount = 0,
            .pSignalSemaphores = nullptr
    };
    vkQueueSubmit(queue, 1, &submit_info, mFence);
    vkWaitForFences(device, 1, &mFence, VK_TRUE, 100000000);

    VkPresentInfoKHR presentInfo {
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .pNext = nullptr,
            .swapchainCount = 1,
            .pSwapchains = &swapchain,
            .pImageIndices = &imageIndex,
            .waitSemaphoreCount = 0,
            .pWaitSemaphores = nullptr,
            .pResults = &result,
    };
    vkQueuePresentKHR(queue, &presentInfo);
}

void VkPipelineHolder::release(const VkDevice& device)
{
    vkFreeCommandBuffers(device, mCommandPool, mCommandBuffers.size(), mCommandBuffers.data());
    vkDestroyCommandPool(device, mCommandPool, nullptr);
    for(int i = 0; i < mFramebuffers.size(); ++i)
    {
        vkDestroyFramebuffer(device, *mFramebuffers[i], nullptr);
        delete mFramebuffers[i];
    }
    vkDestroyShaderModule(device, mVertexShaderModule, nullptr);
    vkDestroyShaderModule(device, mFragmentShaderModule, nullptr);
    vkDestroyPipelineLayout(device, mLayout, nullptr);
    vkDestroyRenderPass(device, mRenderPass, nullptr);
    vkDestroyPipeline(device, mPipeline, nullptr);
}