#include <ImGui/imgui.h>
#include <memory>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "../util/globals/global.hpp"
#include "../util/input/input.hpp"
#include "../imguieditor/fontawesome5.hpp"
#include "camera.hpp"
#include "rigidbody2d.hpp"

namespace Viper::Components {
    Camera::Camera() {
        object = nullptr;
        enabled = true;
    };

    Camera::Camera(GameObject* parent ) {
        object = parent;
        enabled = true;
    };

    void Camera::Awake() {
    };

    float Lerp( float a, float b, float t ) {
        return a + ( b - a ) * t;
    };

    void Camera::Update(Timestep::Timestep ts) {
        auto& tr = object->GetComponent< Transform >( );
        if(!enabled)
            return;
            
        float direction_x = 0.0f;
        float direction_y = 0.0f;
        if( Input::Input::IsKeyPressed(68) )
            direction_x = 1.0f;//tr.position.x += 1.0f;

        if( Input::Input::IsKeyPressed(65) )
            direction_x = -1.0f;//tr.position.x -= 1.0f;

        if( Input::Input::IsKeyPressed(87) )
            direction_y = 1.0f;//tr.position.x += 1.0f;

        if( Input::Input::IsKeyPressed(83) )
            direction_y = -1.0f;//tr.position.x -= 1.0f;
        
        tr.position.x += direction_x * 1.0f * ts.deltatime() * 3.0f;
        tr.position.y += direction_y * 1.0f * ts.deltatime() * 3.0f;
    };

    void Camera::Editor() {
        ImGuiTreeNodeFlags t = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
        if(ImGui::TreeNodeEx( " " ICON_FA_CAMERA "  Camera", t)) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
            ImGui::Checkbox("Enabled##Camera", &enabled);
            ImGui::PopStyleVar();

            ImGui::TreePop();
        };
    };
};