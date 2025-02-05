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
        void LoadModels();
        void CreatePipelineLayout();
        void CreatePipeline();
        void CreateCommandBuffer();
        void DrawFrame();
        
        BEWindow beWindow{WIDTH, HEIGHT, "Bucket Engine"};
        BEDevice beDevice{beWindow};
        BESwapChain beSwapChain{beDevice, beWindow.getExtent()};
        std::unique_ptr<BEPipeline> bePipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkCommandBuffer> commandBuffers;
        std::unique_ptr<BEModel> beModel;
    };
    
}
