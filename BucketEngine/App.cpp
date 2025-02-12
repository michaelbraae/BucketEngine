#include "App.hpp"

#include "renderer/BERenderSystem.hpp"
#include "camera/BECamera.hpp"
#include "input/BEKeyboardMovementController.hpp"
#include "buffers/BEBuffer.hpp"

#include <stdexcept>
#include <array>
#include <chrono>

// TODO:
// Homogeneous coordinates
// descriptor sets

namespace bucketengine
{
    // global uniform buffer object
    struct GlobalUbo
    {
        glm::mat4 projectionView{1.f};
        glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
    };
    
    App::App()
    {
        globalPool = BEDescriptorPool::Builder(beDevice)
            .setMaxSets(BESwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, BESwapChain::MAX_FRAMES_IN_FLIGHT)
            .build();
        
        loadGameObjects();
    }

    App::~App() {}

    void App::run()
    {
        std::vector<std::unique_ptr<BEBuffer>> uboBuffers(BESwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<BEBuffer>(
                beDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            );
            uboBuffers[i]->map();
        }

        // the highest set available to all shaders
        auto globalSetLayout = BEDescriptorSetLayout::Builder(beDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
            .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(BESwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            BEDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        BERenderSystem renderSystem{beDevice, beRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        BECamera camera{};

        auto viewerObject = BEGameObject::createGameObject();
        BEKeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!beWindow.shouldClose())
        {
            // this function call may block, therefore we call it before our delta time calculations
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            // our delta time
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

            // store the current time value for the next iteration of the loop
            currentTime = newTime;

            // handle user input here for the time being until we make a more complete and dynamic implementation
            cameraController.moveInPlaneXZ(beWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            // get the aspect ratio directly from the renderer, so as we resize the viewport
            // our render remains correctly drawn
            float aspect = beRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.f), aspect, .1f, 10.f);

            if (auto commandBuffer = beRenderer.beginFrame())
            {
                int frameIndex = beRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                };

                // update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                beRenderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(frameInfo, gameObjects);
                beRenderer.endSwapChainRenderPass(commandBuffer);
                beRenderer.endFrame();

                // TODO:
                // begin offscreen shadow pass
                // render shadow casting objects
                // end offscreen shadow pass
            }
        }

        vkDeviceWaitIdle(beDevice.device());
    }

    void App::loadGameObjects()
    {
        std::shared_ptr<BEModel> beModel = BEModel::createModelFromFile(beDevice, "models/smooth_vase.obj");

        auto cube = BEGameObject::createGameObject();
        cube.model = beModel;
        cube.transform.translation = {.0f, .2f, .5f};
        cube.transform.scale = {1.f, 1.f, 1.f};

        gameObjects.push_back(std::move(cube));
    }
}
