#include <ImGui/imgui.h>
#include <spdlog/spdlog.h>
#include "boxcollision2d.hpp"
#include "../util/globals/global.hpp"

namespace Viper::Components {
    VIPER_CLASS_DEFINE(Component, BoxCollision2D)

    BoxCollision2D::BoxCollision2D() {
        Box1 = nullptr;
    }

    BoxCollision2D::BoxCollision2D(GameObject *Box1) {
        this->Box1 = Box1;
    }

    void BoxCollision2D::OnAwake() {

    }

    void BoxCollision2D::OnUpdate() {
        Tr = Box1->GetComponent<Transform>();

        for (auto &Objects: Globals::GlobalsContext::Gom->m_GameObjects) {
            if (IsColliding(Objects.get())) {
                Globals::ConsoleContext::AddLog("BoxCollision2D", "Collision Detected!");

                // TODO: Handle this somewhere, right now its just getting sent out but not handled.
                Globals::GlobalsContext::EventHandler->Commit(new BoxCollision2DEvent(Box1, Objects.get()));
            }
        }
    }

    void BoxCollision2D::OnEditor() {
        ImGui::Text("BoxCollision2D");
    }

    bool BoxCollision2D::IsColliding(GameObject *Box2) {
        if (!Box2->HasComponent<Transform>())
            return false;

        if (Box1 == Box2)
            return false;

        Transform Out = Box2->GetComponent<Transform>();

        return CheckCollision(Tr, Out);
    }

    bool BoxCollision2D::CheckCollision(Transform T1, Transform T2) {
        if (T1.position.x < T2.position.x + T2.scale.x &&    // x1 < x2 + w2
            T1.position.x + T1.scale.x > T2.position.x &&       // x1 + w1 > x2
            T1.position.y < T2.position.y + T2.scale.y &&       // y1 < y2 + h2
            T1.position.y + T1.scale.y > T2.position.y) {       // y1 + h1 > y2
            return true; // Collision detected!
        }
        return false;
    }
}