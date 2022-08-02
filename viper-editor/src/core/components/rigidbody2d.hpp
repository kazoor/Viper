#pragma once
#include <glm/vec3.hpp>
#include "component.hpp"
#include "gameobject.hpp"

namespace Viper::Components {
    class Rigidbody2D : public Component {
    public:
        VIPER_COMPONENT_DECLARE( Rigidbody2D );

        Rigidbody2D();

        Rigidbody2D(GameObject* parent);
       
        void Update(Timestep::Timestep ts);

        void AddForce( const glm::vec3& force );

    protected:
        float m_Force;
        float m_Mass;
        float m_Gravity;
        bool grounded;
        bool boundary;
    private:
        GameObject* object;
        glm::vec3 velocity;
    };
};