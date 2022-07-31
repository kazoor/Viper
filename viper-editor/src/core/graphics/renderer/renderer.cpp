#include "renderer.hpp"
#include <array>
#include <glm/mat4x4.hpp>
#include <glad/glad.h>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp> // ortho
#include <graphics/shaders/shader/shader.hpp>

#include "vertexarray.hpp"
#include "vertexbuffer.hpp"
#include "indexbuffer.hpp"
#include "framebuffer.hpp"
#include "linebuffer.hpp"

constexpr uint32_t max_quad_count = 20000;
constexpr uint32_t max_line_count = max_quad_count * 2;
constexpr uint32_t max_vertices_count = max_quad_count * 4;
constexpr uint32_t max_indices_count = max_quad_count * 6;
constexpr uint32_t max_texture_slots = 32; // tdl, fixa någon typ av asset handler

struct RendererData {
    Viper::RendererAPI::Vertex_t* m_VertexBuffer = nullptr;
    Viper::RendererAPI::Vertex_t* m_VertexBufferPtr = nullptr;

    Viper::RendererAPI::LineVertex_t* m_LineVertexBuffer = nullptr;
    Viper::RendererAPI::LineVertex_t* m_LineVertexBufferPtr = nullptr;

    Viper::Ref< Viper::Graphics::Shader > m_QuadShader = nullptr;
    Viper::Ref< Viper::Graphics::Shader > m_LineShader = nullptr;

    uint32_t m_IndexCount = 0;
    uint32_t m_QuadCount = 0;
    uint32_t m_LineCount = 0;

    uint32_t m_PrevIndexCount = 0;
    uint32_t m_PrevQuadCount = 0;
    uint32_t m_PrevLineCount = 0;

    int m_FboWidth = 1280;
    int m_FboHeight = 720;

    glm::vec4 m_QuadTransform[ 4 ];
    glm::vec2 m_TextureTransform[ 4 ];

    Viper::Ref<Viper::Renderer::VertexArray> VertexArray;
    Viper::Ref<Viper::Renderer::VertexArray> LineVertexArray;

    Viper::Ref<Viper::Renderer::IndexBuffer> IndexBuffer;
    Viper::Ref<Viper::Renderer::VertexBuffer> VertexBuffer;
    Viper::Ref<Viper::Renderer::LineBuffer> LineBuffer;
    Viper::Ref<Viper::Renderer::FrameBuffer> FrameBuffer;
    Viper::Ref<Viper::Renderer::OrthoGraphicCamera> OrthoGraphicGamera;
    Viper::Ref<Viper::Renderer::Sprite2D > m_EmptyTexture;
    uint32_t m_TextureSlotIndex = 1;

    std::array< Viper::Ref< Viper::Renderer::Sprite2D >, max_texture_slots > m_TextureSlots;
    glm::mat4 m_ViewProjection = glm::mat4(1.0f);
    glm::mat4 m_FixedTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

};

RendererData s_Renderer;
namespace Viper::Renderer {
    void Renderer2D::Instantiate() {
        s_Renderer.m_VertexBuffer = new Viper::RendererAPI::Vertex_t[ max_quad_count ];
        s_Renderer.m_LineVertexBuffer = new Viper::RendererAPI::LineVertex_t[ max_quad_count ];
        
        s_Renderer.VertexArray = VertexArray::Create();
        s_Renderer.VertexArray->Bind(); // BIND VAO --!> 

        s_Renderer.VertexBuffer = VertexBuffer::Create(max_vertices_count );
        s_Renderer.VertexBuffer->Bind();
        
        s_Renderer.IndexBuffer = IndexBuffer::Create(max_indices_count);
        s_Renderer.IndexBuffer->Bind();

        s_Renderer.VertexArray->Unbind(); // <!-- UNBIND VAO.

        // == // == // == // == // == //

        s_Renderer.LineVertexArray = VertexArray::Create();
        s_Renderer.LineVertexArray->Bind(); // BIND VAO --!>

        s_Renderer.LineBuffer = LineBuffer::Create( max_vertices_count );
        s_Renderer.LineBuffer->Bind();
        s_Renderer.LineVertexArray->Unbind(); // <!-- UNBIND VAO.

        // == // == // == // == // == //

        s_Renderer.FrameBuffer = FrameBuffer::Create();

        //s_Renderer.m_TextureShader = Graphics::Shader::Create("resources/shaders/flat.vert", "resources/shaders/flat.frag");
        s_Renderer.m_QuadShader = Graphics::Shader::Create("resources/shaders/quad.vert", "resources/shaders/quad.frag");
        s_Renderer.m_LineShader = Graphics::Shader::Create("resources/shaders/line.vert", "resources/shaders/line.frag");

        uint32_t whitetexturedata = 0xffffffff;
        s_Renderer.m_EmptyTexture = Sprite2D::Create( 1, 1 );
        s_Renderer.m_EmptyTexture->SetData(&whitetexturedata, sizeof( uint32_t ) );
        s_Renderer.m_QuadShader->Use();
        
        int m_Samplers[ max_texture_slots ];
        for( int i = 0; i < max_texture_slots; i++ )
            m_Samplers[ i ] = i;

        s_Renderer.m_QuadShader->SetIntArray("u_Textures", m_Samplers, max_texture_slots);
        s_Renderer.m_TextureSlots[0] = s_Renderer.m_EmptyTexture;

        // samma logik hos texture.
        s_Renderer.m_QuadTransform[0] = glm::vec4( -0.5f, -0.5f, 0.0f, 1.0f );
	    s_Renderer.m_QuadTransform[1] = glm::vec4(  0.5f, -0.5f, 0.0f, 1.0f );
	    s_Renderer.m_QuadTransform[2] = glm::vec4(  0.5f,  0.5f, 0.0f, 1.0f );
	    s_Renderer.m_QuadTransform[3] = glm::vec4( -0.5f,  0.5f, 0.0f, 1.0f );

        // så detta är samma logik som quad transforms.
        // -0.5f från quadtransform är 0.0f här. 0.5f från quadtransform är 1.0f här.
        s_Renderer.m_TextureTransform[0] = glm::vec2(0.0f, 0.0f);
        s_Renderer.m_TextureTransform[1] = glm::vec2(1.0f, 0.0f);
        s_Renderer.m_TextureTransform[2] = glm::vec2(1.0f, 1.0f);
        s_Renderer.m_TextureTransform[3] = glm::vec2(0.0f, 1.0f);

    };

    void Renderer2D::Destroy() {
        delete[] s_Renderer.m_VertexBuffer;
        delete[] s_Renderer.m_LineVertexBuffer;
    };
    
    void Renderer2D::Flush()
    {
        // Render all the quads.
        {
            GLsizeiptr size_ptr = reinterpret_cast< uint8_t* >( s_Renderer.m_VertexBufferPtr ) - reinterpret_cast< uint8_t* >( s_Renderer.m_VertexBuffer );
            glBindBuffer(GL_ARRAY_BUFFER, s_Renderer.VertexBuffer->Get());
            glBufferSubData(GL_ARRAY_BUFFER, 0, size_ptr, s_Renderer.m_VertexBuffer );

            for (uint32_t i = 0; i < s_Renderer.m_TextureSlotIndex; i++) {
		    		s_Renderer.m_TextureSlots[i]->Bind(i);
            }

            s_Renderer.m_QuadShader->Use();
            s_Renderer.m_QuadShader->SetUniformMat4("u_ViewProjection", s_Renderer.m_ViewProjection);
            s_Renderer.m_QuadShader->SetUniformMat4("u_Transform", s_Renderer.m_FixedTransform);

            RenderCommand::DrawIndexed(s_Renderer.VertexArray->Get(), s_Renderer.m_IndexCount );

            s_Renderer.m_PrevIndexCount = s_Renderer.m_IndexCount;
            s_Renderer.m_PrevQuadCount = s_Renderer.m_QuadCount;
        }

        // Render all the lines.
        {
            GLsizeiptr size_ptr = reinterpret_cast< uint8_t* >( s_Renderer.m_LineVertexBufferPtr ) - reinterpret_cast< uint8_t* >( s_Renderer.m_LineVertexBuffer );
            glBindBuffer(GL_ARRAY_BUFFER, s_Renderer.LineBuffer->Get());
            glBufferSubData(GL_ARRAY_BUFFER, 0, size_ptr, s_Renderer.m_LineVertexBuffer );
            
            s_Renderer.m_LineShader->Use();
            s_Renderer.m_LineShader->SetUniformMat4("u_ViewProjection", s_Renderer.m_ViewProjection);
            s_Renderer.m_LineShader->SetUniformMat4("u_Transform", s_Renderer.m_FixedTransform);

            s_Renderer.LineVertexArray->Bind();
            glDrawArrays(GL_LINES, 0, s_Renderer.m_LineCount);

            s_Renderer.m_PrevLineCount = s_Renderer.m_LineCount;
        }
    }

    void Renderer2D::ResizeFBO( int Width, int Height ) {
        RenderCommand::ResizeTexture( s_Renderer.FrameBuffer->GetTex(), Width, Height );
    };

    void Renderer2D::DrawQuad( const glm::mat4& transform, RendererAPI::Color color ) {
        if( s_Renderer.m_IndexCount >= max_indices_count ) {
            Flush();
            s_Renderer.m_IndexCount = 0;
        
            s_Renderer.m_VertexBufferPtr = s_Renderer.m_VertexBuffer;
            s_Renderer.m_QuadCount = 0;

            s_Renderer.m_LineVertexBufferPtr = s_Renderer.m_LineVertexBuffer;
            s_Renderer.m_LineCount = 0;

            s_Renderer.m_TextureSlotIndex = 1;
        }
            
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

    void Renderer2D::DrawTexture( const glm::vec2& pos, const Ref< Sprite2D >& texture ) {
            auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
             * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f } );

        DrawTexture(m_Transform, texture);
    };

    void Renderer2D::DrawTexture( const glm::vec2& pos, const glm::vec2& size, const Ref< Sprite2D >& texture ) {
            auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
             * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f } );

        DrawTexture(m_Transform, texture);
    };

    void Renderer2D::DrawRotatedTexture( const glm::vec2& pos, float radians, const Ref< Sprite2D >& texture ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
            * glm::rotate(glm::mat4(1.0f), radians, { 0.0f,0.0f,1.0f } )
            * glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f } );
        
        DrawTexture(m_Transform, texture);
    };

    void Renderer2D::DrawRotatedTexture( const glm::vec2& pos, const glm::vec2& size, float radians, const Ref< Sprite2D >& texture ) {
        auto m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) 
            * glm::rotate(glm::mat4(1.0f), radians, { 0.0f,0.0f,1.0f } )
            * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f } );
        
        DrawTexture(m_Transform, texture);
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
        s_Renderer.m_IndexCount = 0;

        s_Renderer.m_VertexBufferPtr = s_Renderer.m_VertexBuffer;
        s_Renderer.m_QuadCount = 0;

        s_Renderer.m_LineVertexBufferPtr = s_Renderer.m_LineVertexBuffer;
        s_Renderer.m_LineCount = 0;

        s_Renderer.m_ViewProjection = camera.GetViewProjectionMatrix();
        s_Renderer.m_TextureSlotIndex = 1;
    };

    void Renderer2D::DrawTexture( const glm::mat4& transform, const Ref< Sprite2D >& texture ) {
        float textureIndex = 0.0f;
		for (uint32_t i = 1; i < s_Renderer.m_TextureSlotIndex; i++)
		{
			if (s_Renderer.m_TextureSlots[i].get() == texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Renderer.m_TextureSlotIndex >= max_texture_slots)
				Flush();

			textureIndex = (float)s_Renderer.m_TextureSlotIndex;
			s_Renderer.m_TextureSlots[s_Renderer.m_TextureSlotIndex] = texture;
			s_Renderer.m_TextureSlotIndex++;
		}
        
        for( int i = 0; i < 4; i++ ) {
            s_Renderer.m_VertexBufferPtr->position = transform * s_Renderer.m_QuadTransform[ i ];
            s_Renderer.m_VertexBufferPtr->color = RendererAPI::Color::White();
            s_Renderer.m_VertexBufferPtr->texcoords = s_Renderer.m_TextureTransform[ i ];
            s_Renderer.m_VertexBufferPtr->texindex = ( float )textureIndex;
            s_Renderer.m_VertexBufferPtr++;
        };
        s_Renderer.m_IndexCount += 6;
        s_Renderer.m_QuadCount++;
    };

    void Renderer2D::DrawLine( const glm::vec3& p0, const glm::vec3& p1, RendererAPI::Color color ) {
        s_Renderer.m_LineVertexBufferPtr->pos = p0;
        s_Renderer.m_LineVertexBufferPtr->color = color;
        s_Renderer.m_LineVertexBufferPtr++;

        s_Renderer.m_LineVertexBufferPtr->pos = p1;
        s_Renderer.m_LineVertexBufferPtr->color = color;
        s_Renderer.m_LineVertexBufferPtr++;

        s_Renderer.m_LineCount += 2;
    };

    uint32_t Renderer2D::GetTexture() {
        return s_Renderer.FrameBuffer->Get();
    };

    void Renderer2D::End() {
        Flush();
    };

    void Renderer2D::BindFramebuffer() {
        RenderCommand::BindFramebuffer(s_Renderer.FrameBuffer->Get());
    };

    void Renderer2D::UnbindFramebuffer() {
        RenderCommand::UnbindFramebuffer();
    };

    uint32_t Renderer2D::GetIndexCount() { return s_Renderer.m_PrevIndexCount; }
    uint32_t Renderer2D::GetQuadCount() { return s_Renderer.m_PrevQuadCount; }
    uint32_t Renderer2D::GetLineCount() { return s_Renderer.m_PrevLineCount; };

    // == // == // == // == // == //

    void RenderCommand::DrawIndexed( uint32_t Vao, uint32_t IndexCount ) {
        glBindVertexArray(Vao);
        glDrawElements(GL_TRIANGLES, IndexCount, GL_UNSIGNED_INT, nullptr );
    };

    void RenderCommand::DrawArrayed( uint32_t Vao, uint32_t IndexCount ) {
        
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

    void RenderCommand::SetClearColor(const glm::vec4& color) {
        glClearColor(color.x, color.y, color.z, color.w);
    };
};