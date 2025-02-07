#pragma once

#include "BEWindow.hpp"
#include "BEPipeline.hpp"
#include "BEGameObject.hpp"
#include "BEDevice.hpp"
#include "BESwapChain.hpp"
#include "BEModel.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <memory>
#include <vector>

namespace bucketengine
{
    // TODO: further reading into alignment, offset and stride assignment
    // When using 32bit float precision, scalar float N = 4 bytes
    // therefore vec2 is 8 bytes
    // in device memory, we require the alignment to be explicit
    struct SimplePushConstantData
    {
        glm::mat2 transform{1.f};
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
        void loadGameObjects();
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void freeCommandBuffers();
        void drawFrame();
        void recreateSwapChain();
        void recordCommandBuffer(int imageIndex);
        void renderGameObjects(VkCommandBuffer commandBuffer);
        
        BEWindow beWindow{WIDTH, HEIGHT, "Bucket Engine"};
        BEDevice beDevice{beWindow};
        std::unique_ptr<BESwapChain> beSwapChain;
        std::unique_ptr<BEPipeline> bePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::vector<BEGameObject> gameObjects;
    };
    
}
