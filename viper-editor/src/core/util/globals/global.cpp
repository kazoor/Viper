#include "global.hpp"
#define VIPER_DECLARE_CONTEXT_NULL( Class, Var ) \
Class GlobalsContext::Var = nullptr; 

namespace Viper::Globals {
    namespace Editor {
        float ZoomLevel = 2.25f;
        float Position[3] = { 0.0f, 0.0f, 0.0f };
        float Radians = 180.f;
        float LightDensity = 1.0f;
        double DeltaTime = 0.0;
        int SelectedObject = -1;
        bool HasMainCamera = false;
        bool isPlaying = false;
        float PosX = 0.0f;
        float PosY = 0.0f;

        float MousePosX = 0.0f;
        float MousePosY = 0.0f;

        float SceneW = 2.0f; 
        float SceneH = 2.0f; 

        float SceneX = 2.0f;
        float SceneY = 2.0f;

        bool IsSceneFocused = false;
        bool IsSceneHovered = false;
    };

    // == // == // == // == // == // == //

    VIPER_DECLARE_CONTEXT_NULL(Components::GameObjectManager*, Gom)

    void GlobalsContext::CreateContext() {
        Gom = new Components::GameObjectManager();
    };

    void GlobalsContext::DestroyContext() {
        delete Gom;
    };

    // == // == // == // == // == // == //

    std::vector< Console_t > ConsoleContext::m_errors{};

    void ConsoleContext::AddLog( const std::string& title, const std::string& log ) {
        m_errors.push_back({title, log, ConsoleFlag::ConsoleNone});
    };

    void ConsoleContext::AddLog( const std::string& title, const std::string& log, ConsoleFlag flag ) {
        m_errors.push_back(Console_t{title, log, flag });
    };

    std::vector< Console_t > ConsoleContext::GetLogs() {
        return m_errors;
    };

    void ConsoleContext::ResizeLogs( const std::size_t maxlogs ) {
        if( m_errors.size( ) >= maxlogs )
            m_errors.erase(m_errors.begin());
    };

    float* ConsoleContext::GetConsoleColor(ConsoleFlag flag) {
        static float color_context[ 3 ];
        switch(flag) {
            case ConsoleFlag::ConsoleNone: {
                color_context[0] = 1.0f, color_context[1] = 1.0f, color_context[2] = 1.0f;
                return color_context;
            }break;

            case ConsoleFlag::ConsoleInfo: {
                color_context[0] = 0.0f, color_context[1] = 0.5f, color_context[2] = 1.0f;
                return color_context;
            }break;

            case ConsoleFlag::ConsoleWarning: {
                color_context[0] = 1.0f, color_context[1] = 0.5f, color_context[2] = 0.0f;
                return color_context;
            }break;

            case ConsoleFlag::ConsoleError: {
                color_context[0] = 1.0f, color_context[1] = 0.2f, color_context[2] = 0.2f;
                return color_context;
            }break;

            case ConsoleFlag::ConsoleSuccess: {
                color_context[0] = 0.0f, color_context[1] = 1.0f, color_context[2] = 0.0f;
                return color_context;
            }break;

        default:
            break;
        };
        return nullptr;
    };
};