#pragma once

#include "BEModel.hpp"

//std
#include <memory>
namespace bucketengine
{
    struct Transform2dComponent
    {
        glm::vec2 tranlation{};
        glm::vec2 scale{1.f, 1.f};

        glm::mat2 mat2()
        {
            glm::mat2 scaleMat{{scale.x, .0f}, {.0f, scale.y}};
            return glm::mat2{1.f};
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
        Transform2dComponent transform2d{};
        
    private:
        BEGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}
