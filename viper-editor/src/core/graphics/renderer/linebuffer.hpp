//
// Created by guwi on 2022-07-26.
//

#ifndef VIPER_LINEBUFFER_HPP
#define VIPER_LINEBUFFER_HPP
#include <cstdint>
#include <vector>
#include <util/ref/reference.hpp>

namespace Viper::Renderer {
    class LineBuffer {
    public:
        LineBuffer( uint32_t max_line_count );
        ~LineBuffer();
        static Ref< LineBuffer > Create( uint32_t max_line_count );
        uint32_t Get() const;
        void Bind();
        void Unbind();
    private:
        uint32_t VBO;
    };
};

#endif //VIPER_LINEBUFFER_HPP
