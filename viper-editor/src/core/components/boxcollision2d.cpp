#include <ImGui/imgui.h>
#include <spdlog/spdlog.h>
#include <util/ref/reference.hpp>
#include "boxcollision2d.hpp"
#include <util/globals/global.hpp>
#include <util/physics/physics2d.hpp>

namespace Viper::Components {
    BoxCollider2D::BoxCollider2D() {
        Box1 = nullptr;
    }

    BoxCollider2D::BoxCollider2D(GameObject *Box1) {
        this->Box1 = Box1;
    }

    void BoxCollider2D::Update(Timestep::Timestep ts) {
        Tr = Box1->GetComponent<Transform>();
        auto& mod = Box1->GetComponent<Transform>();

        for (auto& Objects : *Globals::GlobalsContext::Gom) {
            if (IsColliding(Objects.get())) {
                if(Objects.get() == Box1)
                    continue;
                Globals::ConsoleContext::ResizeLogs(10U);
                Globals::ConsoleContext::AddLog( VIPER_ICON_INFO " BoxCollider2D", 
                VIPER_FORMAT_STRING("GameObject: %s collided with %s", Box1->tag.c_str( ), Objects.get()->tag.c_str( )
                ), Globals::ConsoleFlag::ConsoleInfo);
                //MAKE_REF(BoxCollision2DEvent, (Box1, Objects.get()));
                //Globals::GlobalsContext::EventHandler->Commit( g_BoxCollision2DEvent.get() );//CreateRef< BoxCollision2DEvent >(Box1, Objects.get()));
            }
        }
    }

    void BoxCollider2D::Editor() {

    }

    bool BoxCollider2D::IsColliding(GameObject *Box2) {
        if (!Box2->HasComponent<Transform>())
            return false;
        
        if(!Box2->HasComponent<BoxCollider2D>())
            return false;

        if (Box1 == Box2)
            return false;

        Transform Out = Box2->GetComponent<Transform>();

        //return Physics2D::RectToRect(Tr, Out);

        return true;
    }
}