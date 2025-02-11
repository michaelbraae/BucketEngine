#pragma once

#include "../game/BEGameObject.hpp"
#include "../BEWindow.hpp"

namespace bucketengine
{
    class BEKeyboardMovementController
    {
    public:
        struct KeyMappings
        {
            int moveLeft = GLFW_KEY_A;
            int moveRight = GLFW_KEY_D;
            int moveForward = GLFW_KEY_W;
            int moveBack = GLFW_KEY_S;
            int moveUp = GLFW_KEY_Q;
            int moveDown = GLFW_KEY_E;
            int lookLeft = GLFW_KEY_LEFT;
            int lookRight = GLFW_KEY_RIGHT;
            int lookUp = GLFW_KEY_UP;
            int lookDown = GLFW_KEY_DOWN;
        };

        // currently this will only support GLFW windowing system
        void moveInPlaneXZ(GLFWwindow* window, float deltaTime, BEGameObject& gameObject);

        KeyMappings keys{};
        float moveSpeed{3.f};
        float lookSpeed{1.5f};

    };
}
