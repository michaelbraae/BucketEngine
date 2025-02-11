#include "BEModel.hpp"

// std
#include <cassert>

namespace bucketengine
{
    std::vector<VkVertexInputBindingDescription> BEModel::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        
        return bindingDescriptions;
    }
    
    std::vector<VkVertexInputAttributeDescription> BEModel::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);
        
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);
        
        return attributeDescriptions;
    }
    
    BEModel::BEModel(BEDevice &device, const Builder &builder) : beDevice{device}
    {
        createVertexBuffers(builder.vertices);
        createIndexBuffers(builder.indices);
    }

    BEModel::~BEModel()
    {
        vkDestroyBuffer(beDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(beDevice.device(), vertexBufferMemory, nullptr);

        if (hasIndexBuffer)
        {
            vkDestroyBuffer(beDevice.device(), indexBuffer, nullptr);
            vkFreeMemory(beDevice.device(), indexBufferMemory, nullptr);
        }
    }

    void BEModel::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        // initialise the index buffer if we have one
        if (hasIndexBuffer)
        {
            // for now we use a 32bit index type so we don't have to change the index buffer if a model
            // has too many vertices for 16bit
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }
    }

    void BEModel::draw(VkCommandBuffer commandBuffer)
    {
        if (hasIndexBuffer)
        {
            // works just like the normal draw function, but it signals to vulkan that there's an index buffer
            // available for use
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        } else
        {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void BEModel::createVertexBuffers(const std::vector<Vertex>& vertices)
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
        
        assert(vertexCount >= 3 && "Vertex count must be at least 3");
        
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        
        beDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );
        
        void *data;
        
        // creates a region of host memory, mapped to device memory
        vkMapMemory(beDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(beDevice.device(), stagingBufferMemory);

        beDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            vertexBuffer,
            vertexBufferMemory
        );

        beDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(beDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(beDevice.device(), stagingBufferMemory, nullptr);
    }

    void BEModel::createIndexBuffers(const std::vector<uint32_t> &indices)
    {
        indexCount = static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if (!hasIndexBuffer) return;
        
        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        
        beDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            stagingBuffer,
            stagingBufferMemory
        );
        
        void *data;
        
        // creates a region of host memory, mapped to device memory
        vkMapMemory(beDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(beDevice.device(), stagingBufferMemory);

        beDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            indexBuffer,
            indexBufferMemory
        );

        beDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(beDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(beDevice.device(), stagingBufferMemory, nullptr);
    }
}
