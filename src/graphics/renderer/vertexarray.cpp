//
// Created by guwi on 2022-07-26.
//
#include <glad/glad.h>
#include "vertexarray.hpp"

namespace Viper::Renderer {

    Ref<VertexArray> VertexArray::Create() {
        return Viper::CreateRef<VertexArray>();
    }

    VertexArray::VertexArray() {
        glGenVertexArrays(1, &VAO);
    }

    uint32_t VertexArray::Get() {
        return VAO;
    }

    void VertexArray::Unbind() {
        glBindVertexArray(0);
    }

    void VertexArray::Bind() {
        glBindVertexArray(VAO);
    }
}