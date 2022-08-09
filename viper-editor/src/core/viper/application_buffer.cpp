#include <glad/glad.h>
#include "application_buffer.hpp"
#include <array>
#include <graphics/shaders/shader/shader.hpp>
#include <stb/stb_image.h>

namespace Viper {
    Graphics::Shader* m_TextureShader = nullptr;
    constexpr glm::vec4 quad_transforms[ 4U ] = {
        glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, -1.0f, 0.0f, 1.0f),
        glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
        glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f)
    };

    constexpr glm::vec2 texture_transforms[ 4U ] = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f)
    };

    struct AppBufferVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 uv;
    };

    constexpr float vertices[] = {
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    };

    ApplicationBuffer::ApplicationBuffer() {
        m_TextureShader = new Graphics::Shader("resources/shaders/Renderer2D_Triangle.vert", "resources/shaders/Renderer2D_Triangle.frag");
        
        glCreateVertexArrays(1, &m_Vao);
        glBindVertexArray( m_Vao );

        glCreateBuffers(1, &m_Vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof( float ), ( const void* )offsetof(AppBufferVertex, position) );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof( float ), ( const void* )offsetof(AppBufferVertex, color) );

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof( float ), ( const void* )offsetof(AppBufferVertex, uv) );

        uint32_t m_indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        glCreateBuffers(1, &m_Ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices), m_indices, GL_STATIC_DRAW);

        int m_Samplers[ 10 ];
        for( int i = 0; i < 10; i++ )
            m_Samplers[ i ] = i;
        m_TextureShader->SetIntArray("u_Texture", m_Samplers, 10);
    };

    ApplicationBuffer::~ApplicationBuffer() {
        glDeleteVertexArrays(1, &m_Vao);
        glDeleteBuffers(1, &m_Vbo);

        delete m_TextureShader;
    };

    void ApplicationBuffer::Render(const Ref< FrameBuffer >& framebuffer ) {
        m_TextureShader->Use();
        glBindVertexArray(m_Vao);
        for(int i = 0; i < 10; i++)
            glBindTextureUnit(i, framebuffer->GetColorAttachment());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    };

    Ref< ApplicationBuffer > ApplicationBuffer::Create() {
        return CreateRef< ApplicationBuffer >( );
    };
};