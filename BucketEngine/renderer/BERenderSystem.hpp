#pragma once

#include "../BEWindow.hpp"
#include "../BEPipeline.hpp"
#include "../game/BEGameObject.hpp"
#include "../BEDevice.hpp"
#include "../camera/BECamera.hpp"
#include "BEFrameInfo.hpp"

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
        glm::mat4 modelMatrix{1.f};
        glm::mat4 normalMatrix{1.f};
    };

    class BERenderSystem
    {
    public:
        BERenderSystem(BEDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~BERenderSystem();

        BERenderSystem(const BERenderSystem &) = delete;
        BERenderSystem &operator=(const BERenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo);
    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        BEDevice &beDevice;

        std::unique_ptr<BEPipeline> bePipeline;
        VkPipelineLayout pipelineLayout;
    };
}
