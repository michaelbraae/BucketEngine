#include "App.hpp"

#include <stdexcept>
#include <array>

namespace bucketengine
{
    App::App()
    {
        loadModels();
        createPipelineLayout();
        recreateSwapChain();
        createCommandBuffers();
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
            drawFrame();
        }

        vkDeviceWaitIdle(beDevice.device());
    }

    void App::loadModels()
    {
        std::vector<BEModel::Vertex> vertices {
           {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
           {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
           {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        
        beModel = std::make_unique<BEModel>(beDevice, vertices);
    }

    void App::createPipelineLayout()
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

    void App::createPipeline()
    {
        assert(beSwapChain != nullptr && "Attempting to create pipeline with nullptr swap chain");
        assert(pipelineLayout != nullptr && "Attempting to create pipeline with nullptr pipeline layout");
        PipelineConfigInfo pipelineConfigInfo{};
        BEPipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
        pipelineConfigInfo.renderPass = beSwapChain->getRenderPass();
        pipelineConfigInfo.pipelineLayout = pipelineLayout;
        bePipeline = std::make_unique<BEPipeline>(
            beDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfigInfo
        );
    }

    void App::createCommandBuffers()
    {
        commandBuffers.resize(beSwapChain->imageCount());

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = beDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(beDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers");
        }
    }

    void App::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            beDevice.device(),
            beDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data()
        );
        commandBuffers.clear();
    }

    void App::drawFrame()
    {
        uint32_t imageIndex;
        auto result = beSwapChain->acquireNextImage(&imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        } 

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            // TODO: this can occur when a window is resized, we dont handle that for now
            throw std::runtime_error("Failed to acquire next swap chain image");
        }

        recordCommandBuffer(imageIndex);
        result = beSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || beWindow.getFrameBufferResized())
        {
            beWindow.resetWindowResizedFlag();
            recreateSwapChain();
            return;
        }
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit command buffers");
        }
    }

    void App::recreateSwapChain()
    {
        auto extent = beWindow.getExtent();

        // if the width or the height are equal to zero, we wait
        // this can occur when the window is minimized
        while (extent.width == 0 || extent.height == 0)
        {
            extent = beWindow.getExtent();
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(beDevice.device());

        if (beSwapChain == nullptr)
        {
            beSwapChain = std::make_unique<BESwapChain>(beDevice, extent);
        }
        else
        {
            beSwapChain = std::make_unique<BESwapChain>(beDevice, extent, std::move(beSwapChain));
            if (beSwapChain->imageCount() != commandBuffers.size())
            {
                freeCommandBuffers();
                createCommandBuffers();
            }
        }
        
        createPipeline();
    }

    void App::recordCommandBuffer(int imageIndex)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin command buffer");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = beSwapChain->getRenderPass();
        renderPassInfo.framebuffer = beSwapChain->getFrameBuffer(imageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = beSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(beSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(beSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, beSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
        vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

        bePipeline->bind(commandBuffers[imageIndex]);
        beModel->bind(commandBuffers[imageIndex]);
        beModel->draw(commandBuffers[imageIndex]);

        vkCmdEndRenderPass(commandBuffers[imageIndex]);
        if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to end command buffer");
        }
    }
}
