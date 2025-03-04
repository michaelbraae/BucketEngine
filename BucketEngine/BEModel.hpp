﻿#pragma once

#include "BEDevice.hpp"
#include "buffers/BEBuffer.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <memory>
#include <vector>

namespace bucketengine
{
    class BEModel
    {
    public:
        struct Vertex
        {
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex& other) const
            {
                return position == other.position
                    && color == other.color
                    && normal == other.normal
                    && uv == other.uv;
            }
        };

        struct Builder
        {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void loadModel(const std::string &filePath);
        };

        BEModel(BEDevice &device, const Builder &builder);
        ~BEModel();

        BEModel(const BEModel &) = delete;
        BEModel &operator=(const BEModel &) = delete;

        static std::unique_ptr<BEModel> createModelFromFile(BEDevice &device, const std::string &filePath);
        
        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex> &vertices);
        void createIndexBuffers(const std::vector<uint32_t> &indices);

        BEDevice& beDevice;

        std::unique_ptr<BEBuffer> vertexBuffer;
        // VkBuffer vertexBuffer;
        // VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;

        bool hasIndexBuffer = false;
        std::unique_ptr<BEBuffer> indexBuffer;
        // VkBuffer indexBuffer;
        // VkDeviceMemory indexBufferMemory;
        uint32_t indexCount;
    };
}
