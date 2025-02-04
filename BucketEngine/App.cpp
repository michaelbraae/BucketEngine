#include "App.hpp"

#include <stdexcept>
#include <array>

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
            DrawFrame();
        }

        vkDeviceWaitIdle(beDevice.device());
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
        commandBuffers.resize(beSwapChain.imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = beDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(beDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers");
        }

        for (int i = 0; i < commandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to begin command buffer");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = beSwapChain.getRenderPass();
            renderPassInfo.framebuffer = beSwapChain.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = beSwapChain.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};
            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            bePipeline->Bind(commandBuffers[i]);

            // TODO: for now our vertex shader only has 3 vertices, so we can hard code this
            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to end command buffer");
            }
         }
    }

    void App::DrawFrame()
    {
        uint32_t imageIndex;
        auto result = beSwapChain.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            // TODO: this can occur when a window is resized, we dont handle that for now
            throw std::runtime_error("Failed to acquire next swap chain image");
        }

        result = beSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit command buffers");
        }
    }
}
