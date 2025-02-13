#include "BEPointLightSystem.hpp"

#include <stdexcept>
#include <array>

namespace bucketengine
{
    BEPointLightSystem::BEPointLightSystem(BEDevice& device, VkRenderPass renderPass,
                                           VkDescriptorSetLayout globalSetLayout) : beDevice{device}
    {
        createPipelineLayout(globalSetLayout);
        createPipeline(renderPass);
    }

    BEPointLightSystem::~BEPointLightSystem()
    {
        vkDestroyPipelineLayout(beDevice.device(), pipelineLayout, nullptr);
    }

    void BEPointLightSystem::render(FrameInfo& frameInfo)
    {
        bePipeline->bind(frameInfo.commandBuffer);

        vkCmdBindDescriptorSets(
            frameInfo.commandBuffer,
            VK_PIPELINE_BIND_POINT_GRAPHICS,
            pipelineLayout,
            0,
            1,
            &frameInfo.globalDescriptorSet,
            0,
            nullptr
        );

        vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
    }

    void BEPointLightSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout)
    {
        // VkPushConstantRange pushConstantRange{};
        // pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        // pushConstantRange.offset = 0;
        // pushConstantRange.size = sizeof(SimplePushConstantData);

        std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalSetLayout};

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
        pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(beDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout");
        }
    }

    void BEPointLightSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Attempting to create pipeline with nullptr pipeline layout");
        PipelineConfigInfo pipelineConfigInfo{};
        BEPipeline::defaultPipelineConfigInfo(pipelineConfigInfo);

        // we dont need these values in our point light shaders
        pipelineConfigInfo.attributeDescriptions.clear();
        pipelineConfigInfo.bindingDescriptions.clear();

        pipelineConfigInfo.renderPass = renderPass;
        pipelineConfigInfo.pipelineLayout = pipelineLayout;
        bePipeline = std::make_unique<BEPipeline>(
            beDevice,
            "shaders/lights/point_light.vert.spv",
            "shaders/lights/point_light.frag.spv",
            pipelineConfigInfo
        );
    }
}
