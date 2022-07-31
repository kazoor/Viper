#pragma once
#include <graphics/window/application.hpp>
#include <viper/base.hpp>

namespace API {
    class VIPER_CLASS APIWindow {
    public:
        virtual void Init();
    private:
        Viper::App::Application* m_ExportedApp;
    };
};