#pragma once
#include <string>
#include <vector>
//#include <graphics/renderer/renderer.hpp>
#include <components/gom.hpp>

namespace Viper::Globals {
    class GlobalsContext {
    public:
        static void CreateContext();
        static void DestroyContext();

        static Components::GameObjectManager* Gom;
    };

    enum ConsoleFlag : int {
        ConsoleNone,
        ConsoleInfo,
        ConsoleWarning,
        ConsoleError,
        ConsoleSuccess
    };

    struct Console_t {
        std::string StringType;
        std::string ConsoleMessage;
        ConsoleFlag Flag;
    };

    class ConsoleContext {
    public:
        static void AddLog( const std::string& title, const std::string& log );
        static void AddLog( const std::string& title, const std::string& log, ConsoleFlag flag );

        // Experimental.
        static void ResizeLogs( const std::size_t maxlogs );
        static float* GetConsoleColor(ConsoleFlag flag);

        static std::vector< Console_t > GetLogs();
    private:
        static std::vector< Console_t > m_errors;
    };

    namespace Editor { // Debugging.
        extern float ZoomLevel;
        extern float Position[3];
        extern float Radians;
        extern float LightDensity;
        extern double DeltaTime;
        extern int SelectedObject;
        extern bool HasMainCamera;
        extern bool isPlaying;

        extern float SceneW;
        extern float SceneH;

        extern float SceneX;
        extern float SceneY;

        extern float MousePosX;
        extern float MousePosY;

        extern bool IsSceneFocused;
        extern bool IsSceneHovered;
    };
};