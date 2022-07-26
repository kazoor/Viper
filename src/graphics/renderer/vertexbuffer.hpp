//
// Created by guwi on 2022-07-26.
//

#ifndef VIPER_VERTEXBUFFER_HPP
#define VIPER_VERTEXBUFFER_HPP
#include <cstdint>
#include "../../util/ref/reference.hpp"

namespace Viper::Renderer {
    class VertexBuffer {
    public:
        VertexBuffer();
        static Ref< VertexBuffer > Create( uint32_t max_vert_cound );
    private:
        uint32_t VAO;
        uint32_t VBO;
    };
};

#endif //VIPER_VERTEXBUFFER_HPP
