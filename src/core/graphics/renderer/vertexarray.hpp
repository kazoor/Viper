//
// Created by guwi on 2022-07-26.
//

#ifndef VIPER_VERTEXARRAY_HPP
#define VIPER_VERTEXARRAY_HPP
#include "../../util/ref/reference.hpp"

namespace Viper::Renderer {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        static Ref< VertexArray > Create();

        uint32_t Get() const;

        void Bind();
        void Unbind();
    private:
        uint32_t VAO;
    };
};


#endif //VIPER_VERTEXARRAY_HPP
