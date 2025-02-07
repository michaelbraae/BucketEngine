#include "App.hpp"

#include "BERenderSystem.hpp"

#include <stdexcept>
#include <array>

namespace bucketengine
{
    App::App()
    {
        loadGameObjects();
    }

    App::~App() {}

    void App::run()
    {
        BERenderSystem renderSystem{beDevice, beRenderer.getSwapChainRenderPass()};
        while (!beWindow.shouldClose())
        {
            glfwPollEvents();

            if (auto commandBuffer = beRenderer.beginFrame())
            {
                // begin offscreen shadow pass
                // render shadow casting objects
                // end offscreen shadow pass
                
                beRenderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(commandBuffer, gameObjects);
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
}
