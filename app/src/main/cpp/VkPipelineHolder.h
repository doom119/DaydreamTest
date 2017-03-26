//
// Created by Doom119 on 21/3/9.
//

#ifndef DAYDREAMTEST_VKPIPLELINEHOLDER_H
#define DAYDREAMTEST_VKPIPLELINEHOLDER_H


#include <vulkan/vulkan.h>
#include <vector>
#include <android/asset_manager.h>

class VkPipelineHolder
{
public:
    VkPipelineHolder();
    bool createShader(const VkDevice& device, const AAssetManager* pAssetsManager);
    void createVertexInput();
    void createInputAssembly(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);
    void createViewPort(float x, float y, float width, float height, float minDepth, float maxDepth);
    void createScissor(int32_t x, int32_t y, uint32_t width, uint32_t height);
    void createRasterization(VkPolygonMode polygonMode, float lineWidth, VkCullModeFlags cullMode, VkFrontFace frontFace);
    void createMultisampling();
    void createColorBlend();
    bool createLayout(const VkDevice& device);
    bool createRenderPass(const VkDevice& device, VkFormat format);
    bool createPipeline(const VkDevice& device);

    void release(const VkDevice& device);
private:
    VkPipelineVertexInputStateCreateInfo mVertexInputCreateInfo;
    VkPipelineInputAssemblyStateCreateInfo mInputAssemblyCreateInfo;
    VkPipelineViewportStateCreateInfo mViewportCreateInfo;
    VkPipelineRasterizationStateCreateInfo mRasterizationCreateInfo;
    VkPipelineMultisampleStateCreateInfo mMultisamplingCreateInfo;
    VkPipelineColorBlendStateCreateInfo mColorBlendCreateInfo;
    VkShaderModule mVertexShaderModule;
    VkShaderModule mFragmentShaderModule;
    std::vector<VkPipelineShaderStageCreateInfo> mShaderStageCreateInfos;
    VkPipelineLayout mLayout;
    VkRenderPass mRenderPass;
    VkPipeline mPipeline;
};

#endif //DAYDREAMTEST_VKPIPLELINEHOLDER_H