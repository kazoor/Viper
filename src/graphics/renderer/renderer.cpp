#include "renderer.hpp"
#include <array>
#include <glm/mat4x4.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp> // ortho
#include "../../src/graphics/shaders/shader/shader.hpp"

constexpr uint32_t max_quad_count = 32000;
constexpr uint32_t max_vertices_count = max_quad_count * 4;
constexpr uint32_t max_indices_count = max_quad_count * 6;
constexpr uint32_t max_texture_slots = 32; // tdl, fixa någon typ av asset handler

struct RendererData {
    Viper::RendererAPI::Vertex_t* m_VertexBuffer = nullptr;
    Viper::RendererAPI::Vertex_t* m_VertexBufferPtr = nullptr;

    Viper::Ref< Viper::Graphics::Shader > m_QuadShader = nullptr;
    Viper::Ref< Viper::Graphics::Shader > m_TextureShader = nullptr;

    uint32_t m_IndexCount = 0;
    uint32_t m_QuadCount = 0;
    uint32_t m_VertexCount = 0;

    int m_FboWidth = 1280;
    int m_FboHeight = 720;

    glm::vec4 m_QuadTransform[ 4 ];
    glm::vec2 m_TextureTransform[ 4 ];
};

RendererData s_Renderer;
namespace Viper::Renderer {
    Renderer2D::Renderer2D()
    {
        m_Vao = 0;
        m_Vbo = 0;
        m_Ibo = 0;
        m_Fbo = 0;
        m_Tcb = 0;
        m_TestTexture = 0;

        s_Renderer.m_VertexBuffer = new Viper::RendererAPI::Vertex_t[ max_quad_count ];
        
        glGenVertexArrays(1, &m_Vao);
        glBindVertexArray(m_Vao);

        glGenBuffers(1, &m_Vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, max_vertices_count * sizeof(RendererAPI::Vertex_t), nullptr, GL_DYNAMIC_DRAW);
    
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t), ( const void* )offsetof(RendererAPI::Vertex_t, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t), ( const void* )offsetof(RendererAPI::Vertex_t, color));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t), ( const void* )offsetof(RendererAPI::Vertex_t, texcoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof( RendererAPI::Vertex_t), ( const void* )offsetof(RendererAPI::Vertex_t, texindex));
    
        uint32_t m_IndexArray[ max_indices_count ];
        uint32_t m_IndexOffset = 0;

        for( size_t index = 0; index < max_indices_count; index += 6 ) {
            m_IndexArray[ index + 0 ] = 0 + m_IndexOffset;
            m_IndexArray[ index + 1 ] = 1 + m_IndexOffset;
            m_IndexArray[ index + 2 ] = 2 + m_IndexOffset;
            m_IndexArray[ index + 3 ] = 2 + m_IndexOffset;
            m_IndexArray[ index + 4 ] = 3 + m_IndexOffset;
            m_IndexArray[ index + 5 ] = 0 + m_IndexOffset;

            m_IndexOffset += 4;
        };

        glGenBuffers(1, &m_Ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_IndexArray), m_IndexArray, GL_STATIC_DRAW);

        glBindVertexArray(0);

        glGenFramebuffers(1, &m_Fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Fbo);

        // TESTING. (Skapa textur.)
        glGenTextures(1, &m_Tcb);
        glBindTexture(GL_TEXTURE_2D, m_Tcb);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1280, 720, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Tcb, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenTextures(1, &m_TestTexture);
        glBindTexture(GL_TEXTURE_2D, m_TestTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int m_TexWidth, m_TexHeight, m_TexChannels;
        unsigned char* m_TextureData = stbi_load("cherno.png", &m_TexWidth, &m_TexHeight, &m_TexChannels, 0);

        if( m_TextureData ) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_TexWidth, m_TexHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_TextureData);
            glGenerateMipmap(GL_TEXTURE_2D);
        };

        stbi_image_free(m_TextureData);
        glBindTexture(GL_TEXTURE_2D, 0);

        s_Renderer.m_QuadShader = Graphics::Shader::Create("resources/flat.vert", "resources/flat.frag");
        s_Renderer.m_TextureShader = Graphics::Shader::Create("resources/quad.vert", "resources/quad.frag");

        s_Renderer.m_QuadTransform[0] = glm::vec4( -0.5f, -0.5f, 0.0f, 1.0f );
	    s_Renderer.m_QuadTransform[1] = glm::vec4(  0.5f, -0.5f, 0.0f, 1.0f );
	    s_Renderer.m_QuadTransform[2] = glm::vec4(  0.5f,  0.5f, 0.0f, 1.0f );
	    s_Renderer.m_QuadTransform[3] = glm::vec4( -0.5f,  0.5f, 0.0f, 1.0f );

        s_Renderer.m_TextureTransform[0] = glm::vec2(0.0f, 0.0f);
        s_Renderer.m_TextureTransform[1] = glm::vec2(1.0f, 0.0f);
        s_Renderer.m_TextureTransform[2] = glm::vec2(1.0f, 1.0f);
        s_Renderer.m_TextureTransform[3] = glm::vec2(0.0f, 1.0f);
    }

    Renderer2D::~Renderer2D()
    {
        delete[] s_Renderer.m_VertexBuffer;
        glDeleteFramebuffers(1, &m_Fbo);
        glDeleteVertexArrays(1, &m_Vao);
        glDeleteTextures(1, &m_Tcb);
        glDeleteTextures(1, &m_TestTexture);
        glDeleteBuffers(1, &m_Vbo);
        glDeleteBuffers(1, &m_Ibo);
    }

    void Renderer2D::Flush()
    {
        GLsizeiptr size_ptr = reinterpret_cast< uint8_t* >( s_Renderer.m_VertexBufferPtr ) - reinterpret_cast< uint8_t* >( s_Renderer.m_VertexBuffer );
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size_ptr, s_Renderer.m_VertexBuffer );
    }

    void Renderer2D::ResizeFBO( int Width, int Height ) {
        RenderCommand::ResizeTexture( m_Tcb, Width, Height );
    }

    void Renderer2D::DrawQuad( const glm::vec2& pos, RendererAPI::Color color ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
            * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f } );
            
        for( int i = 0; i < 4; i++ ) {
            s_Renderer.m_VertexBufferPtr->position = m_Transform * s_Renderer.m_QuadTransform[ i ];
            s_Renderer.m_VertexBufferPtr->color = color;
            s_Renderer.m_VertexBufferPtr->texcoords = s_Renderer.m_TextureTransform[ i ];
            s_Renderer.m_VertexBufferPtr->texindex = ( float )0;
            s_Renderer.m_VertexBufferPtr++;
        };

        s_Renderer.m_IndexCount += 6;
        s_Renderer.m_QuadCount++;
    };
    
    void Renderer2D::DrawQuad( const glm::vec2& pos, const glm::vec2& size, RendererAPI::Color color ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f } );

        for( int i = 0; i < 4; i++ ) {
            s_Renderer.m_VertexBufferPtr->position = m_Transform * s_Renderer.m_QuadTransform[ i ];
            s_Renderer.m_VertexBufferPtr->color = color;
            s_Renderer.m_VertexBufferPtr->texcoords = s_Renderer.m_TextureTransform[ i ];
            s_Renderer.m_VertexBufferPtr->texindex = ( float )0;
            s_Renderer.m_VertexBufferPtr++;
        };

        s_Renderer.m_IndexCount += 6;
        s_Renderer.m_QuadCount++;
    };

    void Renderer2D::DrawQuadRotated( const glm::vec2& pos, float radians, RendererAPI::Color color ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
            * glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f } );
        for( int i = 0; i < 4; i++ ) {
            s_Renderer.m_VertexBufferPtr->position = m_Transform * s_Renderer.m_QuadTransform[ i ];
            s_Renderer.m_VertexBufferPtr->color = color;
            s_Renderer.m_VertexBufferPtr->texcoords = s_Renderer.m_TextureTransform[ i ];
            s_Renderer.m_VertexBufferPtr->texindex = ( float )0;
            s_Renderer.m_VertexBufferPtr++;
        }
        s_Renderer.m_IndexCount += 6;
        s_Renderer.m_QuadCount++;
    };

    void Renderer2D::DrawQuadRotated( const glm::vec2& pos, const glm::vec2& size, float radians, RendererAPI::Color color ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
            * glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0.0f, 0.0f, 1.0f))
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f } );
            
        for( int i = 0; i < 4; i++ ) {
            s_Renderer.m_VertexBufferPtr->position = m_Transform * s_Renderer.m_QuadTransform[ i ];
            s_Renderer.m_VertexBufferPtr->color = color;
            s_Renderer.m_VertexBufferPtr->texcoords = s_Renderer.m_TextureTransform[ i ];
            s_Renderer.m_VertexBufferPtr->texindex = ( float )0;
            s_Renderer.m_VertexBufferPtr++;
        }
        s_Renderer.m_IndexCount += 6;
        s_Renderer.m_QuadCount++;
    };

    void Renderer2D::DrawTexture( const glm::vec2& pos, uint32_t textureid ) {
        const auto preset_color = RendererAPI::Color(1.0f,1.0f,1.0f,1.0f);

        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
            * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f } );

        for( int i = 0; i < 4; i++ ) {
            s_Renderer.m_VertexBufferPtr->position = m_Transform * s_Renderer.m_QuadTransform[ i ];
            s_Renderer.m_VertexBufferPtr->color = preset_color;
            s_Renderer.m_VertexBufferPtr->texcoords = s_Renderer.m_TextureTransform[i];
            s_Renderer.m_VertexBufferPtr->texindex = ( float )0;
            s_Renderer.m_VertexBufferPtr++;
        }
        s_Renderer.m_IndexCount += 6;
        s_Renderer.m_QuadCount++;
    };

    void Renderer2D::Begin( const OrthoGraphicCamera& camera ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        s_Renderer.m_VertexBufferPtr = s_Renderer.m_VertexBuffer;

        m_Camera = camera;
       
        s_Renderer.m_QuadShader->Use();
        s_Renderer.m_QuadShader->SetUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
        s_Renderer.m_QuadShader->SetUniformMat4("u_Transform", m_Transform);
    };

    void Renderer2D::End() {

        glBindTexture(GL_TEXTURE_2D, m_Tcb);
        RenderCommand::DrawIndexed(m_Vao, s_Renderer.m_IndexCount );

        s_Renderer.m_IndexCount = 0;
        s_Renderer.m_QuadCount = 0;
        s_Renderer.m_VertexCount = 0;
    };

    void Renderer2D::BindFramebuffer() {
        RenderCommand::BindFramebuffer(m_Fbo);
    };

    void Renderer2D::UnbindFramebuffer() {
        RenderCommand::UnbindFramebuffer();
    };

    OrthoGraphicCamera Renderer2D::GetCamera() const {
        return m_Camera;
    }

    void Renderer2D::PushVec2( const std::string& s, const glm::vec2& v ) {
        s_Renderer.m_QuadShader->SetVector2(s, v);
    };

    void Renderer2D::PushFloat( const std::string& s, float v ) {
        s_Renderer.m_QuadShader->SetFloat(s, v);
    };

    uint32_t Renderer2D::GetVertexCount() { return s_Renderer.m_VertexCount; }
    uint32_t Renderer2D::GetIndexCount() { return s_Renderer.m_IndexCount; }
    uint32_t Renderer2D::GetQuadCount() { return s_Renderer.m_QuadCount; }

    // == // == // == // == // == //

    void RenderCommand::DrawIndexed( uint32_t Vao, uint32_t IndexCount ) {
        glBindVertexArray(Vao);
        glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr );
    };

    void RenderCommand::BindFramebuffer( uint32_t Framebuffer ) {
        glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
    };

    void RenderCommand::UnbindFramebuffer( ) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    };

    void RenderCommand::ResizeTexture( uint32_t TextureID, int Width, int Height ) {
        glBindTexture(GL_TEXTURE_2D, TextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    };
};