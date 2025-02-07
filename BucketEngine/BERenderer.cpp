#include "BERenderer.hpp"

#include <stdexcept>
#include <array>

namespace bucketengine
{
    BERenderer::BERenderer(BEWindow& beWindow, BEDevice& beDevice) :beWindow{beWindow}, beDevice{beDevice}
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    BERenderer::~BERenderer()
    {
        freeCommandBuffers();
    }

    VkCommandBuffer BERenderer::beginFrame()
    {
        assert(!isFrameStarted && "Can't call beginFrame while already in progress");
        
        auto result = beSwapChain->acquireNextImage(&currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            // TODO: this can occur when a window is resized, we dont handle that for now
            throw std::runtime_error("Failed to acquire next swap chain image");
        }

        isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin command buffer");
        }

        return commandBuffer;
    }

    void BERenderer::endFrame()
    {
        assert(isFrameStarted && "Can't call endFrame while frame not in progress");

        auto commandBuffer = getCurrentCommandBuffer();

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to end command buffer");
        }

        auto result = beSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || beWindow.getFrameBufferResized())
        {
            beWindow.resetWindowResizedFlag();
            recreateSwapChain();
        } else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit command buffers");
        }
        
        isFrameStarted = false;
    }

    void BERenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Can't call beginSwapChain while frame not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = beSwapChain->getRenderPass();
        renderPassInfo.framebuffer = beSwapChain->getFrameBuffer(currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = beSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(beSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(beSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, beSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        
    }

    void BERenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(isFrameStarted && "Can't call endSwapChain while frame not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");
        
        vkCmdEndRenderPass(commandBuffer);
    }

    void BERenderer::createCommandBuffers()
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

    void BERenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(
            beDevice.device(),
            beDevice.getCommandPool(),
            static_cast<uint32_t>(commandBuffers.size()),
            commandBuffers.data()
        );
        commandBuffers.clear();
    }

    void BERenderer::recreateSwapChain()
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
        
        // createPipeline();
    }
}
