//
// Created by Doom119 on 21/3/9.
//

#ifndef DAYDREAMTEST_VKPIPLELINEHOLDER_H
#define DAYDREAMTEST_VKPIPLELINEHOLDER_H


#include <vulkan/vulkan.h>
#include <vector>

class VkPipelineHolder
{
public:
    VkPipelineHolder();
    void createVertexInput();
    void createInputAssembly(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);
    void createViewPort(float x, float y, float width, float height, float minDepth, float maxDepth);
    void createScissor(int32_t x, int32_t y, int32_t width, int32_t height);
    void createRasterization(VkPolygonMode polygonMode, float lineWidth, VkCullModeFlags cullMode, VkFrontFace frontFace);
    void createMultisampling();
private:
    VkPipelineVertexInputStateCreateInfo mVertexInputCreateInfo;
    VkPipelineInputAssemblyStateCreateInfo mInputAssemblyCreateInfo;
    VkPipelineViewportStateCreateInfo mViewportCreateInfo;
    VkPipelineRasterizationStateCreateInfo mRasterizationCreateInfo;
    VkPipelineMultisampleStateCreateInfo mMultisamplingCreateInfo;
};

#endif //DAYDREAMTEST_VKPIPLELINEHOLDER_H