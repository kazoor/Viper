#include <viper/viperpch.hpp>
#include <viper/application_buffer.hpp>

#include <random>
#include <chrono>

using namespace Viper::Events;
using namespace Viper::Timestep;
using namespace Viper::Input;

static float m_LastFrame = 0.0f;
class SandboxLayer : public Viper::Layers::Layer {
public:
    SandboxLayer(GLFWwindow* context) : Layer("SandboxLayer"), m_PtrToWindow( context ) {
        Viper::RenderCommand::Init();
        Viper::Renderer2D::Init();
    };

    ~SandboxLayer() {
        Destroy();
    };

    void Destroy() {
        Viper::Renderer2D::Shutdown();
    };

    void OnUpdate( Timestep ts ) {
        Viper::RenderCommand::SetColor({ 0.05f, 0.05f, 0.05f, 1.0f });
        Viper::RenderCommand::Clear();

    };

    void OnEvent(Viper::Events::Event& event) {
        Viper::Events::EventDispatcher dispatch( event );
        dispatch.Dispatch< Viper::Events::MouseCursorPositionEvent >( VIPER_GET_EVENT_FUNC( SandboxLayer::OnMouseMove ) );
    };

    bool OnMouseMove(Viper::Events::MouseCursorPositionEvent& event) {
        printf("mouse moved: %i %i\n", (int)event.x, (int)event.y);

        MouseX = ( int )event.x;
        MouseY = ( int )event.y;
        return true;
    };
private:
    GLFWwindow* m_PtrToWindow;
    float m_Width = 0.0f;
    float m_Height = 0.0f;

    int MouseX = 0;
    int MouseY = 0;
};

class CTest : public Viper::Application {
public:
    CTest() {
    };

    void ApplyLayers(void* appcontext) override {
        PushLayer(new SandboxLayer(reinterpret_cast< GLFWwindow* >( appcontext )));
    };
};
int main() {
    auto editor = new CTest();
    editor->Run();
    
    delete editor;
    return 0;
};