#pragma once

#include "../BEDevice.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>


namespace bucketengine
{
    class BEDescriptorSetLayout
    {
    public:
        class Builder
        {
        public:
            Builder(BEDevice& beDevice) : beDevice{beDevice}
            {
            }

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<BEDescriptorSetLayout> build() const;

        private:
            BEDevice& beDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        BEDescriptorSetLayout(
            BEDevice& beDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~BEDescriptorSetLayout();
        BEDescriptorSetLayout(const BEDescriptorSetLayout&) = delete;
        BEDescriptorSetLayout& operator=(const BEDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        BEDevice& beDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class BEDescriptorWriter;
    };

    class BEDescriptorPool
    {
    public:
        class Builder
        {
        public:
            Builder(BEDevice& beDevice) : beDevice{beDevice}
            {
            }

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<BEDescriptorPool> build() const;

        private:
            BEDevice& beDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        BEDescriptorPool(
            BEDevice& beDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~BEDescriptorPool();
        BEDescriptorPool(const BEDescriptorPool&) = delete;
        BEDescriptorPool& operator=(const BEDescriptorPool&) = delete;

        bool allocateDescriptorSet(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        BEDevice& beDevice;
        VkDescriptorPool descriptorPool;

        friend class BEDescriptorWriter;
    };

    class BEDescriptorWriter
    {
    public:
        BEDescriptorWriter(BEDescriptorSetLayout& setLayout, BEDescriptorPool& pool);

        BEDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        BEDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        BEDescriptorSetLayout& setLayout;
        BEDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };
}
