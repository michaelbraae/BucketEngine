#include "BEMouseInputHandler.hpp"

#include <iostream>

namespace EditorInput
{
    BEMouseInputHandler::BEMouseInputHandler(
        bucketengine::BEWindow& beWindow,
        bucketengine::BECamera& camera,
        bucketengine::BEGameObject& viewerObject,
        bucketengine::BEGameObject::Map& gameObjects
    )
    : beWindow(beWindow), camera(camera), viewerObject(viewerObject), gameObjects(gameObjects)
    {
        glfwSetWindowUserPointer(beWindow.getGLFWwindow(), this);
        glfwSetMouseButtonCallback(beWindow.getGLFWwindow(), mouseButtonCallback);
        std::cout << "Mouse handler constructor";
    }

    BEMouseInputHandler::~BEMouseInputHandler()
    {
    }

    void BEMouseInputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            BEMouseInputHandler* app = static_cast<BEMouseInputHandler*>(glfwGetWindowUserPointer(window));
            if (app) {
                app->pickObject(xpos, ypos);
            }
        }
    }

    bucketengine::BEGameObject& BEMouseInputHandler::pickObject(double xpos, double ypos)
    {
        glm::vec3 rayOrigin = viewerObject.transform.translation;

        // hard coding width for now
        glm::vec3 rayDirection  = screenToWorldRay(
            xpos,
            ypos,
            camera.getView(),
            camera.getProjection(),
            800,
            600
        );

        bucketengine::BEGameObject* closestObject = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        for (auto& [id, obj]: gameObjects)
        {
            glm::vec3 minBounds = obj.transform.translation - obj.transform.scale * 0.5f;
            glm::vec3 maxBounds = obj.transform.translation + obj.transform.scale * 0.5f;

            if (rayIntersectsAABB(rayOrigin, rayDirection, minBounds, maxBounds))
            {
                float distance = glm::length(obj.transform.translation - rayOrigin);
                if (distance < closestDistance)
                {
                    closestDistance = distance;
                    closestObject = &obj;
                }
            }
            // if the ray intersects get our distances
        }

        if (closestObject)
        {
            std::cout << "Selected Object Id: " << closestObject->getId() << "\n";
        } else
        {
            std::cout << "Nothing found... \n";
        }

        return *closestObject;
    }

    glm::vec3 BEMouseInputHandler::screenToWorldRay(
        double xpos,
        double ypos,
        const glm::mat4& viewMatrix,
        const glm::mat4& projMatrix,
        int width,
        int height
    )
    {
        // normalize the device coords (-1.0 -> 1.0)
        float x = (2.f * xpos) / width - 1.f;
        float y = 1.f - (2.f * ypos) / height;
        float z = 1.f;

        // convert to homogeneous clip coords
        glm::vec4 clipCoords = glm::vec4(x, y, -1.f, 1.f);

        // convert to camera space
        glm::vec4 eyeCoords = glm::inverse(projMatrix) * clipCoords;
        eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.f, 0.f);

        // convert to world space
        glm::vec3 worldRay = glm::vec3(glm::inverse(viewMatrix) * eyeCoords);
        return glm::normalize(worldRay);
    }

    bool BEMouseInputHandler::rayIntersectsAABB(
        const glm::vec3& rayOrigin,
        const glm::vec3& rayDirection,
        const glm::vec3& minBounds,
        const glm::vec3& maxBounds
    )
    {
        glm::vec3 invDir = 1.f / rayDirection;
        glm::vec3 t1 = (minBounds - rayOrigin) * invDir;
        glm::vec3 t2 = (maxBounds - rayOrigin) * invDir;

        glm::vec3 tMin = glm::min(t1, t2);
        glm::vec3 tMax = glm::max(t1, t2);

        float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
        float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

        return tNear <= tFar && tFar > 0;
    }
}

