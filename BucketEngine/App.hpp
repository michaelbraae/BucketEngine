#pragma once

#include "BEWindow.hpp"
#include "BEPipeline.hpp"
#include "BEDevice.hpp"
#include "BESwapChain.hpp"
#include "BEModel.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace bucketengine
{
    struct SimplePushConstantData
    {
        glm::vec2 offset;
        alignas(16) glm::vec3 color;
    };
    
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        
        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void run();
    private:
        void loadModels();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        
        BEWindow beWindow{WIDTH, HEIGHT, "Bucket Engine"};
        BEDevice beDevice{beWindow};
        std::unique_ptr<BESwapChain> beSwapChain;
        std::unique_ptr<BEPipeline> bePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<BEModel> beModel;
    };
    
}
