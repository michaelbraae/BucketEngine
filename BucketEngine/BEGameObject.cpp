#include "BEGameObject.hpp"

namespace bucketengine
{
    BEGameObject BEGameObject::createGameObject()
    {
        static id_t currentId = 0;
        return BEGameObject{currentId++};
    }

    BEGameObject::id_t BEGameObject::getId()
    {
        return id;
    }
}
