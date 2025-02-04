#include "App.hpp"

namespace bucketengine
{
    void App::run()
    {
        while (!beWindow.shouldClose())
        {
            glfwPollEvents();
        }
    }
}
