#pragma once

#include "../BEModel.hpp"

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
        glm::mat4 mat4();
        glm::mat3 normalMatrix();
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
