#include "App.hpp"

#include "renderer/BERenderSystem.hpp"
#include "camera/BECamera.hpp"
#include "input/BEKeyboardMovementController.hpp"

#include <stdexcept>
#include <array>
#include <chrono>

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

            // store the current time value for the next iteration of the while loop
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
                // begin offscreen shadow pass
                // render shadow casting objects
                // end offscreen shadow pass

                beRenderer.beginSwapChainRenderPass(commandBuffer);
                renderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                beRenderer.endSwapChainRenderPass(commandBuffer);
                beRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(beDevice.device());
    }

    void App::loadGameObjects()
    {
        std::shared_ptr<BEModel> beModel = createCubeModel(beDevice, {.0f, .0f, .0f});

        auto cube = BEGameObject::createGameObject();
        cube.model = beModel;
        cube.transform.translation = {.0f, .0f, 2.5f};
        cube.transform.scale = {1.f, 1.f, 1.f};

        gameObjects.push_back(std::move(cube));
    }

    std::unique_ptr<BEModel> App::createCubeModel(BEDevice& device, glm::vec3 offset) {
        BEModel::Builder modelBuilder{};
        modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
 
            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
 
            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
 
            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
 
            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
 
            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };
        for (auto& v : modelBuilder.vertices) {
            v.position += offset;
        }
 
        modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};
 
        return std::make_unique<BEModel>(device, modelBuilder);
    }
}
