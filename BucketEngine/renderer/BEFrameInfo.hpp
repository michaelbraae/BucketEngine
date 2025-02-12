#pragma once

#include "../camera/BECamera.hpp"

#include <vulkan/vulkan.h>

namespace bucketengine
{
    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        BECamera &camera;
    };    
}
