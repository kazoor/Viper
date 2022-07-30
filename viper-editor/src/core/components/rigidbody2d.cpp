#include <ImGui/imgui.h>
#include <memory>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "../util/globals/global.hpp"
#include "../util/input/input.hpp"
#include "../imguieditor/fontawesome5.hpp"
#include "rigidbody2d.hpp"

namespace Viper::Components {
    Rigidbody2D::Rigidbody2D() {
        object = nullptr;
        enabled = true;
        m_Force = 100.0f;
        m_Mass = 100.0f;
        grounded = false;
        m_Gravity = 2.0f;
        velocity = glm::vec3( 0.0f, 0.0f, 0.0f );
        boundary = true;
    };

    Rigidbody2D::Rigidbody2D(GameObject* parent ) {
        object = parent;
        enabled = true;
        m_Force = 100.0f;
        m_Mass = 100.0f;
        m_Gravity = 2.0f;
        grounded = false;
        velocity = glm::vec3( 0.0f, 0.0f, 0.0f );
        boundary = true;
    };

    void Rigidbody2D::Awake() {
    };

    void Rigidbody2D::Update(Timestep::Timestep ts) {
        if(!enabled) {
            velocity = glm::vec3( 0.0f, 0.0f, 0.0f );
            return;
        }
            //return;
        auto& tr = object->GetComponent< Transform >( );

        if( tr.position.y < -30.0f ) {
            tr.position.y = 0.0f;
            velocity = glm::vec3( 0.0f, 0.0f, 0.0f );
        }
        
        tr.position.y += velocity.y * ts;
        velocity.y += -m_Gravity * ts;
    };

    void Rigidbody2D::AddForce( const glm::vec3& force ) {
        velocity = force;
    };

    void Rigidbody2D::Editor() {
        ImGuiTreeNodeFlags t = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
        if(ImGui::TreeNodeEx( " " ICON_FA_GOLF_BALL "  Rigidbody2D", t)) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
            ImGui::Checkbox("Enabled##Rigidbody2D", &enabled);
            ImGui::PopStyleVar();

            ImGui::DragFloat( "Force", &m_Force );
            ImGui::DragFloat( "Mass", &m_Mass );
            ImGui::DragFloat( "Gravity", &m_Gravity );

            ImGui::Checkbox( "Boundary", &boundary);

            ImGui::TreePop();
        };
    };
};