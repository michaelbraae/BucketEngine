#pragma once

#include "../BEWindow.hpp"
#include "../game/BEGameObject.hpp"
#include "../camera/BECamera.hpp"

namespace EditorInput
{
    class BEMouseInputHandler
    {
    public:
        BEMouseInputHandler(
            bucketengine::BEWindow& beWindow,
            bucketengine::BECamera& camera,
            bucketengine::BEGameObject& viewerObject,
            bucketengine::BEGameObject::Map& gameObjects
        );
        ~BEMouseInputHandler();

    private:
        bucketengine::BEWindow& beWindow;
        bucketengine::BECamera& camera;
        bucketengine::BEGameObject& viewerObject;
        bucketengine::BEGameObject::Map& gameObjects;
        
        static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

        bucketengine::BEGameObject& pickObject(double xpos, double ypos);

        glm::vec3 screenToWorldRay(
            double xpos,
            double ypos,
            const glm::mat4& viewMatrix,
            const glm::mat4& projMatrix,
            int width,
            int height
        );

        bool rayIntersectsAABB(
            const glm::vec3& rayOrigin,
            const glm::vec3& rayDirection,
            const glm::vec3& minBounds,
            const glm::vec3& maxBounds
        );
    };
}
