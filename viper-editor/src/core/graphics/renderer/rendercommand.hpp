#pragma once
#include <glm/vec4.hpp>
#include <viper/viper.hpp>
#include <util/ref/reference.hpp>

#include <viper/viper.hpp>

namespace Viper {
    class VIPER_API RenderCommand {
    public:
        static void Init();
        
        static void Clear();
        static void SetColor(const glm::vec4 color);
        static void Resize(const int width, const int height);

        static void BindFBO();
        static void UnbindFBO();

        static uint32_t FrameID( );
    };
};