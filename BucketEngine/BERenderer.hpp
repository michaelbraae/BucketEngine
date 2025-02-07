#pragma once

#include "BEWindow.hpp"
#include "BEDevice.hpp"
#include "BESwapChain.hpp"
#include "BEModel.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <cassert>
#include <memory>
#include <vector>

namespace bucketengine
{
    class BERenderer
    {
    public:
        BERenderer(BEWindow& beWindow, BEDevice& beDevice);
        ~BERenderer();

        BERenderer(const BERenderer &) = delete;
        BERenderer &operator=(const BERenderer &) = delete;

        bool isFrameInProgress() const { return isFrameStarted; }

        VkRenderPass getSwapChainRenderPass() const { return beSwapChain->getRenderPass(); }
        VkCommandBuffer getCurrentCommandBuffer() const
        {
            assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
            return commandBuffers[currentImageIndex];
        }

        VkCommandBuffer beginFrame();
        void endFrame();

        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
        
    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();
        
        BEWindow& beWindow;
        BEDevice& beDevice;
        std::unique_ptr<BESwapChain> beSwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        bool isFrameStarted;
    };
    
}
