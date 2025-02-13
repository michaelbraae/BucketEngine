#pragma once

#include "../camera/BECamera.hpp"
#include "../game/BEGameObject.hpp"

// lib
#include <vulkan/vulkan.h>

namespace bucketengine
{
    struct FrameInfo
    {
        int frameIndex;
        float frameTime;
        VkCommandBuffer commandBuffer;
        BECamera &camera;
        VkDescriptorSet globalDescriptorSet;
        BEGameObject::Map &gameObjects;
    };
}
