#pragma once

#include "BucketEngineDevice.hpp"
#include <string>
#include <vector>

namespace bucketengine
{
    struct PipelineConfigInfo
    {
        VkViewport viewport;
        VkRect2D scissor;
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };
    
    class BEPipeline
    {
    public:
        BEPipeline(
            BucketEngineDevice &device,
            const std::string vertFilePath,
            const std::string fragFilePath,
            const PipelineConfigInfo& configInfo
        );
        ~BEPipeline();

        BEPipeline(const BEPipeline&) = delete;
        void operator=(const BEPipeline&) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> ReadFile(const std::string& filepath);

        void CreateGraphicsPipeline(
            const std::string vertFilePath,
            const std::string fragFilePath,
            const PipelineConfigInfo& configInfo
        );

        void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        BucketEngineDevice& beDevice;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}
