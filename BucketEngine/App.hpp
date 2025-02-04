#pragma once

#include "BucketEngineWindow.hpp"
#include "BucketEnginePipeline.hpp"
#include "BucketEngineDevice.hpp"
#include "BucketEngineSwapChain.hpp"

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
        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffer();
        void DrawFrame();
        
        BEWindow beWindow{WIDTH, HEIGHT, "HELLO WORLD"};
        BucketEngineDevice beDevice{beWindow};
        BucketEngineSwapChain beSwapChain{beDevice, beWindow.getExtent()};
        std::unique_ptr<BEPipeline> bePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
    };
    
}
