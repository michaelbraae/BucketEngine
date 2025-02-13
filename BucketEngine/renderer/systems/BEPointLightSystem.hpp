#pragma once

#include "../../BEWindow.hpp"
#include "../../BEPipeline.hpp"
#include "../../game/BEGameObject.hpp"
#include "../../BEDevice.hpp"
#include "../../camera/BECamera.hpp"
#include "../BEFrameInfo.hpp"

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
    class BEPointLightSystem
    {
    public:
        BEPointLightSystem(BEDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~BEPointLightSystem();

        BEPointLightSystem(const BEPointLightSystem &) = delete;
        BEPointLightSystem &operator=(const BEPointLightSystem &) = delete;

        void render(FrameInfo &frameInfo);
    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        BEDevice &beDevice;

        std::unique_ptr<BEPipeline> bePipeline;
        VkPipelineLayout pipelineLayout;
    };
}
