#include "BEWindow.hpp"

#include <stdexcept>

namespace bucketengine
{
    BEWindow::BEWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name}
    {
        initWindow();
    }

    BEWindow::~BEWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void BEWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
             throw std::runtime_error("Failed to create window surface");
        }
    }

    void BEWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height)
    {
        auto beWindow = reinterpret_cast<BEWindow *>(glfwGetWindowUserPointer(window));
        beWindow->frameBufferResized = true;
        beWindow->width = width;
        beWindow->height = height;
    }

    void BEWindow::initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
    }
}
