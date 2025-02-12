﻿#pragma once

#include "BEWindow.hpp"
#include "game/BEGameObject.hpp"
#include "BEDevice.hpp"
#include "renderer/BERenderer.hpp"
#include "BEModel.hpp"
#include "descriptors/BEDescriptors.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <memory>
#include <vector>

namespace bucketengine
{
    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
        
        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void run();
    private:
        void loadGameObjects();

        BEWindow beWindow{WIDTH, HEIGHT, "Bucket Engine"};
        BEDevice beDevice{beWindow};
        BERenderer beRenderer{beWindow, beDevice};

        std::unique_ptr<BEDescriptorPool> globalPool{};
        
        BEGameObject::Map gameObjects;
    };
    
}
