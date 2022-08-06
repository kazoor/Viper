#pragma once
#include <glm/mat4x4.hpp>

namespace Viper {
    class Camera {  
    public:
        Camera() = default;
        Camera( const glm::mat4& projection );
        virtual ~Camera() = default;
        const glm::mat4& GetProjection() const;
    protected:
        glm::mat4 m_Projection;
    };
};