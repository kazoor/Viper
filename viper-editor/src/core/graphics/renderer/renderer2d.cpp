#include <glad/glad.h>
#include "renderer2d.hpp"
#include <util/ref/reference.hpp>
#include <graphics/shaders/shader/shader.hpp>
#include <cstring>
#include <util/timer/timer.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 uvcoords;
    float texture_index;
    float texture_tiling;
};

struct LineVertex {
    glm::vec3 position;
    glm::vec4 color;
};

namespace Viper {
    // Quad related
    uint32_t m_Vao = 0;
    uint32_t m_Vbo = 0;
    uint32_t m_Ibo = 0;

    // Line related
    uint32_t m_LineVao = 0;
    uint32_t m_LineVbo = 0;

    // pointers
    Vertex* m_Ptr = nullptr;
    Vertex* m_PtrBuff = nullptr;

    LineVertex* m_LinePtr = nullptr;
    LineVertex* m_LinePtrBuff = nullptr;

    Graphics::Shader* m_ShaderDebug__Square = nullptr;
    Graphics::Shader* m_ShaderDebug__Line = nullptr;
    
    Renderer::OrthoGraphicCamera render_camera;

    constexpr const uint32_t m_MaxQuads = 20000;
    constexpr const uint32_t m_MaxVertices = m_MaxQuads * 4;
    constexpr const uint32_t m_MaxIndices = m_MaxQuads * 6;
    constexpr const uint32_t m_MaxLines = m_MaxQuads * 2;
    static const uint32_t m_MaxTextures = 32;

    uint32_t m_IndexCount = 0;
    uint32_t m_LineCount = 0;
    static uint32_t m_TextureSlotIndex = 1;
    Stats stats;

    constexpr auto quad_corners = 4U;
    constexpr glm::vec4 quad_transforms[ 4U ] = {
        glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
        glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
        glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)
    };

    constexpr glm::vec2 texture_transforms[ 4U ] = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(0.0f, 1.0f)
    };

    std::array< Ref< Sprite2D >, m_MaxTextures > m_TextureSlots;
    Ref< Sprite2D > m_PlaceHolder;

    Stats& Renderer2D::GetStats() {
        return stats;
    };

    void Renderer2D::Init() {
        m_ShaderDebug__Square = new Graphics::Shader( "resources/shaders/quad.vert", "resources/shaders/quad.frag");
        m_ShaderDebug__Line = new Graphics::Shader( "resources/shaders/line.vert", "resources/shaders/line.frag");

        m_Ptr = new Vertex[ m_MaxVertices ];
        m_LinePtr = new LineVertex[ m_MaxVertices ];

        // // // // // // Quad Renderer // // // // // //
        glCreateVertexArrays(1, &m_Vao);
        glBindVertexArray(m_Vao);
        
        glCreateBuffers(1, &m_Vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, m_MaxVertices * sizeof( Vertex ), nullptr, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof( Vertex, position ) );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof( Vertex, color ) );

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof( Vertex, uvcoords ) );

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof( Vertex, texture_index ) );

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof( Vertex, texture_tiling ) );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        uint32_t* m_indices = new uint32_t[ m_MaxIndices ];
        uint32_t m_indice_offset = 0u;
        for( size_t index = 0; index < m_MaxIndices; index += 6 ) {
            m_indices[ index + 0 ] = m_indice_offset + 0;
            m_indices[ index + 1 ] = m_indice_offset + 1;
            m_indices[ index + 2 ] = m_indice_offset + 2;

            m_indices[ index + 3 ] = m_indice_offset + 2;
            m_indices[ index + 4 ] = m_indice_offset + 3;
            m_indices[ index + 5 ] = m_indice_offset + 0;

            m_indice_offset += 4;
        };

        glCreateBuffers(1, &m_Ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_MaxIndices * sizeof( uint32_t ), m_indices, GL_STATIC_DRAW);
        delete[] m_indices;
        
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // // // // // Texture Renderer // // // // // //

        // // // // // Line Renderer // // // // // //
        //glGenVertexArrays(1, &m_LineVao);
        glCreateVertexArrays(1, &m_LineVao);
        glBindVertexArray(m_LineVao);

        glCreateBuffers(1, &m_LineVbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_LineVbo);
        glBufferData(GL_ARRAY_BUFFER, m_MaxVertices * sizeof( LineVertex ), nullptr, GL_DYNAMIC_DRAW );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( LineVertex ), ( const void* )offsetof( LineVertex, position ) );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( LineVertex ), ( const void* )offsetof( LineVertex, color) );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindVertexArray( 0 );

        m_PlaceHolder = Sprite2D::Create( 1, 1 );
        uint32_t m_WhiteData = 0xffffffff;
        m_PlaceHolder->SetData(&m_WhiteData);
        m_ShaderDebug__Square->Use();

        int samplers[ m_MaxTextures ];
        for( int i = 0; i < m_MaxTextures; i++ )
            samplers[ i ] = i;

        m_ShaderDebug__Square->SetIntArray( "u_Textures", samplers, m_MaxTextures );
        m_TextureSlots[ 0 ] = m_PlaceHolder; // reserve 0.
    };

    void Renderer2D::Shutdown() {
        delete m_Ptr;
        delete m_LinePtr;

        delete m_ShaderDebug__Square;
        delete m_ShaderDebug__Line;

        glDeleteVertexArrays(1, &m_Vao);
        glDeleteVertexArrays(1, &m_LineVao);

        glDeleteBuffers(1, &m_Vbo);
        glDeleteBuffers(1, &m_LineVbo);

        glDeleteBuffers(1, &m_Ibo);
    };

    void Renderer2D::DrawQuad( const glm::vec2& pos, const glm::vec2& size, glm::vec4 color) {
        glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), glm::vec3( pos.x, pos.y, 0.0f ) ) * glm::scale( glm::mat4( 1.0f ), glm::vec3( size.x, size.y, 1.0f ) );
        DrawQuad(transform, color);
    };

    void Renderer2D::DrawQuadRotated(const glm::vec2& pos, const glm::vec2& size, float angle, glm::vec4 color) {
        glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), glm::vec3( pos.x, pos.y, 0.0f ) ) * glm::rotate( glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale( glm::mat4( 1.0f ), glm::vec3( size.x, size.y, 1.0f ) );
        DrawQuad( transform, color );
    };

    void Renderer2D::DrawQuad( const glm::mat4& transform, glm::vec4 color ) {
        if( m_IndexCount >= m_MaxIndices ) {
            Flush();
            BeginBatch();
        };

        constexpr float texture_fixed_index = 0.0f;

        for( size_t index = 0; index < quad_corners; index++ ) {
            m_PtrBuff->position = transform * quad_transforms[ index ];
            m_PtrBuff->color = color;
            m_PtrBuff->uvcoords = texture_transforms[ index ];
            m_PtrBuff->texture_index = texture_fixed_index;
            m_PtrBuff->texture_tiling = 1.0f;
            m_PtrBuff++;
        };

        m_IndexCount += 6;
    };

    void Renderer2D::DrawTexture( const glm::mat4& transform, const Ref< Sprite2D >& sprite, float tiling, glm::vec4 color) {
        if( m_IndexCount >= m_MaxIndices ) {
            Flush();
            BeginBatch();
        };

        float textureIndex = 0.0f;
		for (uint32_t i = 1; i < m_TextureSlotIndex; i++)
		{
			if (m_TextureSlots[i].get() == sprite.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (m_TextureSlotIndex >= m_MaxTextures)
				Flush();

			textureIndex = (float)m_TextureSlotIndex;
			m_TextureSlots[m_TextureSlotIndex] = sprite;
			m_TextureSlotIndex++;
		}

        for( size_t index = 0; index < quad_corners; index++ ) {
            m_PtrBuff->position = transform * quad_transforms[ index ];
            m_PtrBuff->color = color;
            m_PtrBuff->uvcoords = texture_transforms[ index ];
            m_PtrBuff->texture_index = ( float )textureIndex;
            m_PtrBuff->texture_tiling = tiling;
            m_PtrBuff++;
        };

        m_IndexCount += 6;
    };
    void Renderer2D::DrawTexture( const glm::vec2& pos, const glm::vec2& size, const Ref< Sprite2D >& sprite, float tiling, glm::vec4 color) {
        glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), glm::vec3( pos.x, pos.y, 0.0f ) ) * glm::scale( glm::mat4( 1.0f ), glm::vec3( size.x, size.y, 1.0f ) );
        DrawTexture(transform, sprite, tiling, color);
    };

    void Renderer2D::DrawRotatedTexture( const glm::vec2& pos, const glm::vec2& size, const Ref< Sprite2D >& sprite, float tiling, float angle, glm::vec4 color) {
        glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), glm::vec3( pos.x, pos.y, 0.0f ) ) * glm::rotate( glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale( glm::mat4( 1.0f ), glm::vec3( size.x, size.y, 1.0f ) );
        DrawTexture(transform, sprite, tiling, color);
    };

    void Renderer2D::DrawLine( const glm::vec2& pos0, const glm::vec2& pos1, glm::vec4 color) {
        m_LinePtrBuff->position = glm::vec3( pos0.x, pos0.y, 0.0f );
        m_LinePtrBuff->color = color;
        m_LinePtrBuff++;

        m_LinePtrBuff->position = glm::vec3( pos1.x, pos1.y, 0.0f );
        m_LinePtrBuff->color = color;
        m_LinePtrBuff++;

        m_LineCount += 2;
    };

    void Renderer2D::DrawOutlinedQuad( const glm::vec2& pos, const glm::vec2& size, glm::vec4 color ) {
        DrawLine({ pos.x, pos.y }, { pos.x + size.x, pos.y }, color ); // top
    };


    void Renderer2D::Flush() {
        // Render all the quads.
        if( m_IndexCount ) {
            const auto& size_ptr = ( uint8_t* )m_PtrBuff - ( uint8_t* )m_Ptr;
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size_ptr, m_Ptr );
            
            for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		    	m_TextureSlots[i]->Bind(i);

            m_ShaderDebug__Square->Use();
            m_ShaderDebug__Square->SetUniformMat4("u_ViewProjection", render_camera.GetViewProjectionMatrix( ) * glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ) ) );
            
            const uint32_t index_count = m_IndexCount ? m_IndexCount : m_MaxIndices;
            glBindVertexArray(m_Vao);
            glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr );
        };

        // Render all the lines.
        if( m_LineCount ) {
            const auto& size_ptr = ( uint8_t* )m_LinePtrBuff - ( uint8_t* )m_LinePtr;
            glBindBuffer(GL_ARRAY_BUFFER, m_LineVbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size_ptr, m_LinePtr );
           
            m_ShaderDebug__Line->Use();
            m_ShaderDebug__Line->SetUniformMat4("u_ViewProjection", render_camera.GetViewProjectionMatrix( ) * glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ) ) );

            const uint32_t line_count = m_LineCount ? m_LineCount : m_MaxLines;
            glBindVertexArray(m_LineVao);
            glDrawArrays(GL_LINES, 0, line_count);
        };

        stats.calls_this_frame++;
        stats.max_indices_allowed = m_MaxIndices;
        stats.max_vertices_allowed = m_MaxVertices;
        stats.max_quads_allowed = m_MaxQuads;
    };

    void Renderer2D::BeginBatch() {

        m_IndexCount = 0;
        m_PtrBuff = m_Ptr;

        m_LineCount = 0;
        m_LinePtrBuff = m_LinePtr;

        m_TextureSlotIndex = 1;
    };

    /* onödig funktion: tabort. */
    void Renderer2D::EndBatch() { };

    void Renderer2D::Begin( const Renderer::OrthoGraphicCamera& camera ) {
        render_camera = camera;
        std::memset(&stats, 0, sizeof( Stats ) );

        BeginBatch();
    };

    void Renderer2D::End() {
        Flush();
    };
};