﻿#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace bucketengine
{
    class BEWindow
    {
    public:
        BEWindow(int w, int h, std::string name);
        ~BEWindow();

        BEWindow(const BEWindow &) = delete;
        BEWindow &operator=(const BEWindow &) = delete;

        bool shouldClose() { return glfwWindowShouldClose(window); }

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    private:
        void initWindow();

        const int width;
        const int height;

        std::string windowName;
        GLFWwindow *window;
    };
    
}
