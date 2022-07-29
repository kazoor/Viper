#include "renderer.hpp"
#include <array>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp> // ortho
#include <graphics/shaders/shader/shader.hpp>

#include "vertexarray.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "framebuffer.hpp"

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

    uint32_t m_PrevIndexCount = 0;
    uint32_t m_PrevQuadCount = 0;

    int m_FboWidth = 1280;
    int m_FboHeight = 720;

    glm::vec4 m_QuadTransform[ 4 ];
    glm::vec2 m_TextureTransform[ 4 ];

    Viper::Ref<Viper::Renderer::VertexArray> VertexArray;
    Viper::Ref<Viper::Renderer::IndexBuffer> IndexBuffer;
    Viper::Ref<Viper::Renderer::VertexBuffer> VertexBuffer;
    Viper::Ref<Viper::Renderer::FrameBuffer> FrameBuffer;
    Viper::Ref<Viper::Renderer::OrthoGraphicCamera> OrthoGraphicGamera;
};

RendererData s_Renderer;
namespace Viper::Renderer {
    void Renderer2D::Instantiate() {
        s_Renderer.m_VertexBuffer = new Viper::RendererAPI::Vertex_t[ max_quad_count ];
        
        s_Renderer.VertexArray = VertexArray::Create();
        s_Renderer.VertexArray->Bind();

        s_Renderer.VertexBuffer = VertexBuffer::Create(max_vertices_count * sizeof(RendererAPI::Vertex_t));
        s_Renderer.VertexBuffer->Bind();
        
        s_Renderer.IndexBuffer = IndexBuffer::Create(max_indices_count);
        s_Renderer.IndexBuffer->Bind();

        s_Renderer.VertexArray->Unbind();

        s_Renderer.FrameBuffer = FrameBuffer::Create();

        s_Renderer.m_QuadShader = Graphics::Shader::Create("resources/shaders/flat.vert", "resources/shaders/flat.frag");
        s_Renderer.m_TextureShader = Graphics::Shader::Create("resources/shaders/quad.vert", "resources/shaders/quad.frag");

        s_Renderer.m_QuadTransform[0] = glm::vec4( -0.5f, -0.5f, 0.0f, 1.0f );
	    s_Renderer.m_QuadTransform[1] = glm::vec4(  0.5f, -0.5f, 0.0f, 1.0f );
	    s_Renderer.m_QuadTransform[2] = glm::vec4(  0.5f,  0.5f, 0.0f, 1.0f );
	    s_Renderer.m_QuadTransform[3] = glm::vec4( -0.5f,  0.5f, 0.0f, 1.0f );

        s_Renderer.m_TextureTransform[0] = glm::vec2(0.0f, 0.0f);
        s_Renderer.m_TextureTransform[1] = glm::vec2(1.0f, 0.0f);
        s_Renderer.m_TextureTransform[2] = glm::vec2(1.0f, 1.0f);
        s_Renderer.m_TextureTransform[3] = glm::vec2(0.0f, 1.0f);
    };

    void Renderer2D::Destroy() {
        delete[] s_Renderer.m_VertexBuffer;
    };
    
    void Renderer2D::Flush()
    {
        GLsizeiptr size_ptr = reinterpret_cast< uint8_t* >( s_Renderer.m_VertexBufferPtr ) - reinterpret_cast< uint8_t* >( s_Renderer.m_VertexBuffer );
        glBindBuffer(GL_ARRAY_BUFFER, s_Renderer.VertexBuffer->Get());
        glBufferSubData(GL_ARRAY_BUFFER, 0, size_ptr, s_Renderer.m_VertexBuffer );

        s_Renderer.m_PrevIndexCount = s_Renderer.m_IndexCount;
        s_Renderer.m_PrevQuadCount = s_Renderer.m_QuadCount;
    }

    void Renderer2D::ResizeFBO( int Width, int Height ) {
        RenderCommand::ResizeTexture( s_Renderer.FrameBuffer->GetTex(), Width, Height );
    };

    void Renderer2D::DrawQuad( const glm::mat4& transform, RendererAPI::Color color ) {
        for( int i = 0; i < 4; i++ ) {
            s_Renderer.m_VertexBufferPtr->position = transform * s_Renderer.m_QuadTransform[ i ];
            s_Renderer.m_VertexBufferPtr->color = color;
            s_Renderer.m_VertexBufferPtr->texcoords = s_Renderer.m_TextureTransform[ i ];
            s_Renderer.m_VertexBufferPtr->texindex = ( float )0;
            s_Renderer.m_VertexBufferPtr++;
        };
        s_Renderer.m_IndexCount += 6;
        s_Renderer.m_QuadCount++;
    };

    void Renderer2D::DrawQuad( const glm::vec2& pos, RendererAPI::Color color ) {
            auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
             * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f } );

        DrawQuad(m_Transform, color);
    };
    void Renderer2D::DrawQuad( const glm::vec2& pos, const glm::vec2& size, RendererAPI::Color color ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
             * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f } );

        DrawQuad(m_Transform, color);
    };

    void Renderer2D::DrawQuadRotated( const glm::vec2& pos, float radians, RendererAPI::Color color ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
            * glm::rotate(glm::mat4(1.0f), radians, { 0.0f,0.0f,1.0f } )
            * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f } );
        
        DrawQuad(m_Transform, color);
    };

    void Renderer2D::DrawQuadRotated( const glm::vec2& pos, const glm::vec2& size, float radians, RendererAPI::Color color ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
            * glm::rotate(glm::mat4(1.0f), radians, { 0.0f,0.0f,1.0f } )
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f } );
        
        DrawQuad(m_Transform, color);
    };

    void Renderer2D::Begin( const OrthoGraphicCamera& camera ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        s_Renderer.m_VertexBufferPtr = s_Renderer.m_VertexBuffer;
       
        s_Renderer.m_QuadShader->Use();
        s_Renderer.m_QuadShader->SetUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        s_Renderer.m_QuadShader->SetUniformMat4("u_Transform", m_Transform);
    };

    uint32_t Renderer2D::GetTexture() {
        return s_Renderer.FrameBuffer->Get();
    };

    void Renderer2D::End() {

        glBindTexture(GL_TEXTURE_2D, s_Renderer.FrameBuffer->GetTex());
        RenderCommand::DrawIndexed(s_Renderer.VertexArray->Get(), s_Renderer.m_IndexCount );

        s_Renderer.m_IndexCount = 0;
        s_Renderer.m_QuadCount = 0;
    };

    void Renderer2D::BindFramebuffer() {
        RenderCommand::BindFramebuffer(s_Renderer.FrameBuffer->Get());
    };

    void Renderer2D::UnbindFramebuffer() {
        RenderCommand::UnbindFramebuffer();
    };

    uint32_t Renderer2D::GetIndexCount() { return s_Renderer.m_PrevIndexCount; }
    uint32_t Renderer2D::GetQuadCount() { return s_Renderer.m_PrevQuadCount; }

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
    }

    void RenderCommand::Clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    };

    void RenderCommand::SetClearColor(float color[3]) {
        glClearColor(color[0], color[1], color[2], color[3]);
    };
};