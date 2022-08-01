#include "editor.hpp"
#include <viper/application.hpp>
#include <imguieditor/imguieditor.hpp>
#include <imguieditor/scene/scenelayer.hpp>

class CTest : public Viper::Application {
public:
    CTest() {
    };

    void ApplyLayers(void* appcontext) override {
        //PushOverlay(new Viper::ImGuiEditor(appcontext));
        PushLayer(new Viper::SceneLayer(appcontext));
    };
};
int main() {
    auto editor = new CTest();
    editor->Run();
    
    delete editor;
    return 0;
};