#pragma once

#include "BEWindow.hpp"
#include "BucketEnginePipeline.hpp"
#include "BucketEngineDevice.hpp"

namespace bucketengine
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();

    private:
        BEWindow beWindow{WIDTH, HEIGHT, "HELLO WORLD"};
        BucketEngineDevice beDevice{beWindow};
        BEPipeline bePipeline{beDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", BEPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
    
}
