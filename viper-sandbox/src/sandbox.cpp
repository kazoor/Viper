#include <viper/viperpch.hpp>
#include <viper/application_buffer.hpp>
#include <graphics/renderer/renderer3d.hpp>
#include <glm/glm.hpp>

#include <random>
#include <chrono>

using namespace Viper::Events;
using namespace Viper::Timestep;
using namespace Viper::Input;

static float m_LastFrame = 0.0f;
class SandboxLayer : public Viper::Layers::Layer {
public:
    SandboxLayer(GLFWwindow* context) : Layer("SandboxLayer"), m_PtrToWindow( context ) {
        Viper::Renderer3D::Init();
    };

    ~SandboxLayer() {
        Destroy();
    };

    void Destroy() {
        Viper::Renderer3D::Shutdown();
    };

    void OnUpdate( Timestep ts ) {
        Viper::RenderCommand::SetColor({ 0.05f, 0.05f, 0.05f, 1.0f });
        Viper::RenderCommand::Clear();

        //Viper::Renderer3D::Quad();
        //Viper::Renderer3D::Begin();
        static float m_Angle = 0.0f;
        m_Angle += 10.0f * ts.deltatime() * 6.0f;

        auto trans = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f) ) * glm::scale( glm::mat4(1.0f), glm::vec3(1.0f,1.0f,1.0f) ) * glm::rotate( glm::mat4( 1.0f ), glm::radians( m_Angle ), glm::vec3(1, 1, 1 ));
        Viper::Renderer3D::Quad(trans, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

        auto trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, 0.2f, 0.0f) ) * glm::scale( glm::mat4(1.0f), glm::vec3(0.1f,0.1f,1.0f) ) * glm::rotate( glm::mat4( 1.0f ), glm::radians( m_Angle ), glm::vec3(1, 1, 1 ));
        Viper::Renderer3D::Quad(trans2, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
        Viper::Renderer3D::End();
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