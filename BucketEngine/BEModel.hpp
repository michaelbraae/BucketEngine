#pragma once

#include "BEDevice.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

namespace bucketengine
{
    class BEModel
    {
    public:
        struct Vertex
        {
            glm::vec2 position;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };
        
        BEModel(BEDevice &device, const std::vector<Vertex> &vertices);
        ~BEModel();

        BEModel(const BEModel &) = delete;
        BEModel &operator=(const BEModel &) = delete;

        void Bind(VkCommandBuffer commandBuffer);
        void Draw(VkCommandBuffer commandBuffer);
        
    private:
        void CreateVertexBuffers(const std::vector<Vertex> &vertices);
        
        BEDevice& beDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;
    };
}
