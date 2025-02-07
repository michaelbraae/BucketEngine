#include "App.hpp"

#include <stdexcept>
#include <array>

namespace bucketengine
{
    App::App()
    {
        loadGameObjects();
        createPipelineLayout();
        createPipeline();
    }

    App::~App()
    {
        vkDestroyPipelineLayout(beDevice.device(), pipelineLayout, nullptr);
    }

    void App::run()
    {
        while (!beWindow.shouldClose())
        {
            glfwPollEvents();

            if (auto commandBuffer = beRenderer.beginFrame())
            {
                // begin offscreen shadow pass
                // render shadow casting objects
                // end offscreen shadow pass
                
                beRenderer.beginSwapChainRenderPass(commandBuffer);
                renderGameObjects(commandBuffer);
                beRenderer.endSwapChainRenderPass(commandBuffer);
                beRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(beDevice.device());
    }

    void App::loadGameObjects()
    {
        std::vector<BEModel::Vertex> vertices {
           {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
           {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
           {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
        
        auto beModel = std::make_shared<BEModel>(beDevice, vertices);
        
        auto triangle = BEGameObject::createGameObject();
        triangle.model = beModel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.tranlation.x = .2f;
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();
        
        gameObjects.push_back(std::move(triangle));
    }

    void App::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantData);
        
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        
        if (vkCreatePipelineLayout(beDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout");
        }
    }

    void App::createPipeline()
    {
        assert(pipelineLayout != nullptr && "Attempting to create pipeline with nullptr pipeline layout");
        PipelineConfigInfo pipelineConfigInfo{};
        BEPipeline::defaultPipelineConfigInfo(pipelineConfigInfo);
        pipelineConfigInfo.renderPass = beRenderer.getSwapChainRenderPass();
        pipelineConfigInfo.pipelineLayout = pipelineLayout;
        bePipeline = std::make_unique<BEPipeline>(
            beDevice,
            "shaders/simple_shader.vert.spv",
            "shaders/simple_shader.frag.spv",
            pipelineConfigInfo
        );
    }

    void App::renderGameObjects(VkCommandBuffer commandBuffer)
    {
        bePipeline->bind(commandBuffer);

        for (auto& obj: gameObjects)
        {
            obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.01f, glm::two_pi<float>());
            
            SimplePushConstantData push{};
            push.offset = obj.transform2d.tranlation;
            push.color = obj.color;
            push.transform = obj.transform2d.mat2();

            vkCmdPushConstants(
                commandBuffer,
                pipelineLayout,
                VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
                0,
                sizeof(SimplePushConstantData),
                &push
            );

            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }
}
