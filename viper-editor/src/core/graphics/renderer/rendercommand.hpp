#pragma once
#include <glm/vec4.hpp>
#include <viper/viper.hpp>
#include <util/ref/reference.hpp>

#include <viper/viper.hpp>

namespace Viper {
    class VIPER_API RenderCommand {
    public:
        static void Clear();
        static void SetColor(const glm::vec4 color);
    };
};