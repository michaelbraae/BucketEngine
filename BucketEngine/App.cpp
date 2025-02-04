#include "App.hpp"

#include <stdexcept>

namespace bucketengine
{
    App::App()
    {
        CreatePipelineLayout();
        CreatePipeline();
        CreateCommandBuffer();
    }

    App::~App()
    {
        vkDestroyPipelineLayout(beDevice.device(), pipelineLayout, nullptr);
    }

    void App::run()
    {
        while (!beWindow.shouldClose())
        {
            glfwPollEvents();
        }
    }

    void App::CreatePipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;
        
        if (vkCreatePipelineLayout(beDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout");
        }
    }

    void App::CreatePipeline()
    {
        auto pipelineConfig = BEPipeline::defaultPipelineConfigInfo(beSwapChain.width(), beSwapChain.height());
        pipelineConfig.renderPass = beSwapChain.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout;
        bePipeline = std::make_unique<BEPipeline>(
            beDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfig
        );
    }

    void App::CreateCommandBuffer()
    {
    }

    void App::DrawFrame()
    {
    }
}
