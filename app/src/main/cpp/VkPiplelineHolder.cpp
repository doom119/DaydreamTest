//
// Created by Doom119 on 21/3/9.
//

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

bool VkPipelineHolder::createShader(const VkDevice &device)
{
    //todo
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
    VkViewport viewport = {
            .x = x,
            .y = y,
            .width = width,
            .height = height,
            .minDepth = minDepth,
            .maxDepth = maxDepth
    };
    mViewportCreateInfo.viewportCount = 1;
    mViewportCreateInfo.pViewports = &viewport;
}

void VkPipelineHolder::createScissor(int32_t x, int32_t y, uint32_t width, uint32_t height)
{
    VkRect2D scissor = {
            .offset.x = x,
            .offset.y = y,
            .extent.width = width,
            .extent.height = height
    };
    mViewportCreateInfo.scissorCount = 1;
    mViewportCreateInfo.pScissors = &scissor;
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
}

bool VkPipelineHolder::createPipeline(const VkDevice &device)
{
    //todo
}

void VkPipelineHolder::release(const VkDevice& device)
{
    vkDestroyPipelineLayout(device, mLayout, nullptr);
    vkDestroyRenderPass(device, mRenderPass, nullptr);
}