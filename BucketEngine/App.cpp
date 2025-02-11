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
        std::shared_ptr<BEModel> beModel = BEModel::createModelFromFile(beDevice, "models/smooth_vase.obj");

        auto cube = BEGameObject::createGameObject();
        cube.model = beModel;
        cube.transform.translation = {.0f, .2f, .5f};
        cube.transform.scale = {1.f, 1.f, 1.f};

        gameObjects.push_back(std::move(cube));
    }
}
