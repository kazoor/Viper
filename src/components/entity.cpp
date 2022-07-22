#include "entity.hpp"

namespace Viper::Components {
    size_t Entity::GetComponentSize() const {
        return m_Components.size();
    };

    Transform Entity::GetTransform() const {
        return transform;
    };

    const char* Entity::GetTag() const {
        return tag;
    };
};