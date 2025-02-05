#pragma once

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

        VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
        
        bool getFrameBufferResized() { return frameBufferResized; }
        void resetWindowResizedFlag() { frameBufferResized = false; }
        
        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    private:
        static void frameBufferResizedCallback(GLFWwindow *window, int width, int height);
        void initWindow();

        int width;
        int height;
        bool frameBufferResized = false;

        std::string windowName;
        GLFWwindow *window;
    };
    
}
