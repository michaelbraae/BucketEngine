#pragma once

#include "../BEWindow.hpp"
#include "../BEPipeline.hpp"
#include "../game/BEGameObject.hpp"
#include "../BEDevice.hpp"
#include "../camera/BECamera.hpp"

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
        glm::mat4 transform{1.f};
        alignas(16) glm::vec3 color;
    };
    
    class BERenderSystem
    {
    public:
        
        BERenderSystem(BEDevice &device, VkRenderPass renderPass);
        ~BERenderSystem();

        BERenderSystem(const BERenderSystem &) = delete;
        BERenderSystem &operator=(const BERenderSystem &) = delete;

        void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<BEGameObject> &gameObjects, const BECamera& camera);
    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);
        
        BEDevice &beDevice;
        
        std::unique_ptr<BEPipeline> bePipeline;
        VkPipelineLayout pipelineLayout;
    };
}
