#include "camera.hpp"

namespace Viper {
    Camera::Camera( const glm::mat4& projection ) : m_Projection(projection) {

    };

    const glm::mat4& Camera::GetProjection() const {
        return m_Projection;
    };
}