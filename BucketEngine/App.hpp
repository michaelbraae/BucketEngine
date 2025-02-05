#pragma once

#include "BEWindow.hpp"
#include "BEPipeline.hpp"
#include "BEDevice.hpp"
#include "BESwapChain.hpp"
#include "BEModel.hpp"

// std
#include <memory>
#include <vector>

namespace bucketengine
{
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
        void createCommandBuffer();
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
