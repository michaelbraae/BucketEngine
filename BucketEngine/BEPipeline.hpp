#pragma once

#include "BEDevice.hpp"
#include "BEModel.hpp"

// std
#include <string>
#include <vector>

namespace bucketengine
{
    struct PipelineConfigInfo
    {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

        std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

        VkPipelineViewportStateCreateInfo viewportInfo;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;

        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;

        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;

        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class BEPipeline
    {
    public:
        BEPipeline(
            BEDevice& device,
            const std::string vertFilePath,
            const std::string fragFilePath,
            const PipelineConfigInfo& configInfo
        );
        ~BEPipeline();

        BEPipeline(const BEPipeline&) = delete;
        BEPipeline& operator=(const BEPipeline&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

    private:
        static std::vector<char> readFile(const std::string& filepath);

        void createGraphicsPipeline(
            const std::string vertFilePath,
            const std::string fragFilePath,
            const PipelineConfigInfo& configInfo
        );

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        BEDevice& beDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}
