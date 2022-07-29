//
// Created by guwi on 2022-07-26.
//

#ifndef VIPER_INDEXBUFFER_HPP
#define VIPER_INDEXBUFFER_HPP
#include <cstdint>
#include <util/ref/reference.hpp>

namespace Viper::Renderer {
    class IndexBuffer {
    public:
        IndexBuffer( uint32_t max_index_count );
        ~IndexBuffer();

        uint32_t Get() const;

        void Bind();
        void Unbind();

        static Ref< IndexBuffer > Create( uint32_t max_index_count );
    private:
        uint32_t IBO;
    };
};

#endif //VIPER_INDEXBUFFER_HPP
