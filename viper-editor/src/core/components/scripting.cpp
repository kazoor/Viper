#include "scripting.hpp"
#include "camera.hpp"
#include "../viper/base.hpp"

namespace Viper::Components {
    Scripting::Scripting() {
        parent = nullptr;
    };

    Scripting::Scripting(GameObject* object) {
        parent = object;
    };

    void Scripting::Update(double deltatime) {

    };

    void Scripting::Awake() {

    };

    bool Scripting::Begin() {
        ImGuiTreeNodeFlags t = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Framed;
        auto frame = ImGui::TreeNodeEx( VIPER_FORMAT_STRING( " " ICON_FA_CODE "  Script :: %s", script_name.c_str( ) ).c_str( ) , t);
        return frame;
    };

    void Scripting::End() {
        ImGui::TreePop();
    };

    void Scripting::OnGui() { };

    void Scripting::Editor() {
        if(Begin()) {
            OnGui();
            End();
        };
    };

    template< typename T >
    T& Scripting::GetComponent() {
        return parent->GetComponent< T >( );
    };

    template< typename T, typename... TArgs >
    void Scripting::AddComponent(TArgs&&... args) {
        parent->AddComponent< T >( std::forward< TArgs >( args )... );
    };

    template< typename T >
    bool Scripting::HasComponent() const {
        return parent->HasComponent< T >( );
    };

    template< typename T >
    void Scripting::RemoveComponent() {
        parent->RemoveComponent< T >( );
    };

    TestScript::TestScript() {
        parent = nullptr;
        ScriptDescriptor("TestScript");
    };
    
    TestScript::TestScript(GameObject* object) {
        parent = object;
        ScriptDescriptor("TestScript");
    };

    void TestScript::OnGui() {
        ImGui::Text("hello kajzan");

        if( !HasComponent< Camera >( ) ) {
            if( ImGui::Button( "Add Camera!" ) )
                AddComponent< Camera >( parent );
        } else {
            if( ImGui::Button( "Remove Camera!" ) )
                RemoveComponent< Camera >( );
        };        
    };
};