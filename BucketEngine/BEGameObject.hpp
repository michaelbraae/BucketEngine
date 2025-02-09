#pragma once

#include "BEModel.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

//std
#include <memory>

namespace bucketengine
{
    struct TransformComponent
    {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation{};

        // these rotations are interpreted as Extrinsic by reading them right to left (z -> y -> x)
        // ie: around the global origin.
        // to interpret the rotation as Intrinsic, we read the angles from left to right instead (x -> y -> z)
        // ie: around the object itself
        glm::mat4 mat4() {
            const float c3 = glm::cos(rotation.z);
            const float s3 = glm::sin(rotation.z);
            const float c2 = glm::cos(rotation.x);
            const float s2 = glm::sin(rotation.x);
            const float c1 = glm::cos(rotation.y);
            const float s1 = glm::sin(rotation.y);
            return glm::mat4{
            {
                scale.x * (c1 * c3 + s1 * s2 * s3),
                scale.x * (c2 * s3),
                scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                scale.y * (c3 * s1 * s2 - c1 * s3),
                scale.y * (c2 * c3),
                scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                scale.z * (c2 * s1),
                scale.z * (-s2),
                scale.z * (c1 * c2),
                0.0f,
            },
            {translation.x, translation.y, translation.z, 1.0f}};
        }
    };
    // the root class for all game objects
    class BEGameObject
    {
    public:
        using id_t = unsigned int;

        static BEGameObject createGameObject();

        id_t getId();

        BEGameObject(const BEGameObject &) = delete;
        BEGameObject &operator=(const BEGameObject &) = delete;
        BEGameObject(BEGameObject&&) = default;
        BEGameObject &operator=(BEGameObject&&) = default;

        std::shared_ptr<BEModel> model{};
        glm::vec3 color{};
        TransformComponent transform{};
        
    private:
        BEGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}
