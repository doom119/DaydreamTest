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

void VkPipelineHolder::createScissor(int32_t x, int32_t y, int32_t width, int32_t height)
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