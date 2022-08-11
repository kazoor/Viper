#include <cstring>
#include <glad/glad.h>
#include "renderer2d.hpp"
#include <util/ref/reference.hpp>
#include <graphics/shaders/shader/shader.hpp>
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

struct TriangleVertex {
    glm::vec3 position;
    glm::vec4 color;
};

namespace Viper {
    class IndexBuffer {
    public:
        IndexBuffer( uint32_t* indices, uint32_t count ) : m_Count( count ) {
            glCreateBuffers( 1, &m_Ibo );

            glBindBuffer(GL_ARRAY_BUFFER, m_Ibo);
            glBufferData(GL_ARRAY_BUFFER, count * sizeof( uint32_t ), indices, GL_STATIC_DRAW );
        };

        ~IndexBuffer() {
            glDeleteBuffers(1, &m_Ibo);
        };

        void Bind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
        };

        void Unbind() const {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        };

        const uint32_t GetCount() const { return m_Count; };

        static Ref< IndexBuffer > Create( uint32_t* indices, uint32_t count ) {
            return CreateRef< IndexBuffer >( indices, count );
        };
    private:
        uint32_t m_Ibo;
        uint32_t m_Count;
    };

    class VertexBuffer {
    public:
        VertexBuffer( const uint32_t maxbuffer, bool isDynamic ) {
            glCreateBuffers(1, &m_Vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo );
            glBufferData(GL_ARRAY_BUFFER, maxbuffer, nullptr, isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW );
        };

        VertexBuffer( float* vertices, const uint32_t maxbuffer ) {
            glCreateBuffers(1, &m_Vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo );
            glBufferData(GL_ARRAY_BUFFER, maxbuffer, vertices, GL_STATIC_DRAW );
        };

        ~VertexBuffer() {
            glDeleteBuffers(1, &m_Vbo);
        };

        void Bind() const {
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        };

        void Unbind() const {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        };

        void SetData( const void* data, uint32_t size ) {
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, data );
        };

        static Ref< VertexBuffer > Create( const uint32_t maxbuffer, bool isDynamic = true ) {
            return CreateRef< VertexBuffer >( maxbuffer, isDynamic );
        };

        static Ref< VertexBuffer > Create( float* vertices, const uint32_t maxbuffer ) {
            return CreateRef< VertexBuffer >( vertices, maxbuffer );
        };
    private:
        uint32_t m_Vbo;
        uint32_t m_MaxSize;
    };

    class VertexArray {
    public:
        VertexArray( ) {
            glCreateVertexArrays(1, &m_Vao);
        };

        ~VertexArray() {
            glDeleteVertexArrays(1, &m_Vao);
        };

        void Bind() const {
            glBindVertexArray(m_Vao);
        };

        void SetQuadInfo(const Ref< VertexBuffer >& vertexbuffer ) const {
            glBindVertexArray(m_Vao);
		    vertexbuffer->Bind();

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
        };

        void SetLineInfo(const Ref< VertexBuffer >& vertexbuffer) const {
            glBindVertexArray(m_Vao);
            vertexbuffer->Bind();

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( LineVertex ), ( const void* )offsetof( LineVertex, position ) );

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( LineVertex ), ( const void* )offsetof( LineVertex, color) );
        };

        void Unbind() const {
            glBindVertexArray(0);
        };

        static Ref< VertexArray > Create( ) {
            return CreateRef< VertexArray >( );
        };

        void SetIndexBuffer( const Ref< IndexBuffer >& indexbuffer ) {
            glBindVertexArray(m_Vao);
            indexbuffer->Bind();

            m_IndexBuffer = indexbuffer;
        };

        const Ref< IndexBuffer >& GetIndexBuffer() const { return m_IndexBuffer; };
    private:
        uint32_t m_MaxSize = 0;
        uint32_t m_Vao;

        Ref< IndexBuffer > m_IndexBuffer;
    };

    enum class VertexType : int {
        ARRAY_BUFFER = GL_ARRAY_BUFFER,
        ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
        ARRAY_BUFFER_BINDING = GL_ARRAY_BUFFER_BINDING,
        ELEMENT_ARRAY_BUFFER_BINDING = GL_ELEMENT_ARRAY_BUFFER_BINDING,
        VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = GL_ELEMENT_ARRAY_BUFFER_BINDING
    };

    // pointers
    Vertex* m_Ptr = nullptr;
    Vertex* m_PtrBuff = nullptr;

    LineVertex* m_LinePtr = nullptr;
    LineVertex* m_LinePtrBuff = nullptr;

    TriangleVertex* m_TriPtr = nullptr;
    TriangleVertex* m_TriPtrBuff = nullptr;

    Graphics::Shader* m_ShaderDebug__Square = nullptr;
    Graphics::Shader* m_ShaderDebug__Line = nullptr;
    Graphics::Shader* m_ShaderDebug__Triangle = nullptr;
    
    Renderer::OrthoGraphicCamera render_camera;

    constexpr const uint32_t m_MaxQuads = 20000;
    constexpr const uint32_t m_MaxVertices = m_MaxQuads * 4;
    constexpr const uint32_t m_MaxIndices = m_MaxQuads * 6;
    constexpr const uint32_t m_MaxLines = m_MaxQuads * 2;
    constexpr const uint32_t m_MaxTri = m_MaxQuads * 3;
    static const uint32_t m_MaxTextures = 32;

    uint32_t m_IndexCount = 0;
    uint32_t m_LineCount = 0;
    uint32_t m_TriangleCount = 0;

    static uint32_t m_TextureSlotIndex = 1;
    Stats stats;

    constexpr auto quad_corners = 4U;
    constexpr glm::vec4 quad_transforms[ 4U ] = {
        glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
        glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
        glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f)
    };

    constexpr glm::vec4 triangle_transforms[ 3U ] = {
        glm::vec4( -0.5f, -0.5f, 0.0f, 1.0f ),
        glm::vec4(  0.5f, -0.5f, 0.0f, 1.0f ),
        glm::vec4(  0.0f,  0.5f, 0.0f, 1.0f )
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

    Ref<VertexArray> QuadVertexArray;
    Ref<VertexBuffer> QuadVertexBuffer;


    Ref<VertexArray> LineVertexArray;
    Ref<VertexBuffer> LineVertexBuffer;

    void Renderer2D::Init() {
        m_ShaderDebug__Square = new Graphics::Shader( "resources/shaders/Renderer2D_Quad.vert", "resources/shaders/Renderer2D_Quad.frag");
        m_ShaderDebug__Line = new Graphics::Shader( "resources/shaders/Renderer2D_Line.vert", "resources/shaders/Renderer2D_Line.frag");
        m_ShaderDebug__Triangle = new Graphics::Shader( "resources/shaders/Renderer2D_Triangle.vert", "resources/shaders/Renderer2D_Triangle.frag");

        m_Ptr = new Vertex[ m_MaxVertices ];
        m_LinePtr = new LineVertex[ m_MaxVertices ];
        m_TriPtr = new TriangleVertex[ m_MaxVertices ];

        QuadVertexArray = VertexArray::Create();
        QuadVertexBuffer = VertexBuffer::Create( m_MaxVertices * sizeof(Vertex) );
        QuadVertexArray->SetQuadInfo(QuadVertexBuffer);

        uint32_t* quad_indices = new uint32_t[m_MaxIndices];
        uint32_t quad_offset = 0;

        for( uint32_t index = 0; index < m_MaxIndices; index += 6 ) {
            quad_indices[ index + 0 ] = quad_offset + 0;
            quad_indices[ index + 1 ] = quad_offset + 1;
            quad_indices[ index + 2 ] = quad_offset + 2;

            quad_indices[ index + 3 ] = quad_offset + 2;
            quad_indices[ index + 4 ] = quad_offset + 3;
            quad_indices[ index + 5 ] = quad_offset + 0;

            quad_offset += 4;
        };

        Ref<IndexBuffer> quadIB = IndexBuffer::Create(quad_indices, m_MaxIndices);
        QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quad_indices;


        LineVertexArray = VertexArray::Create();
        LineVertexBuffer = VertexBuffer::Create( m_MaxVertices * sizeof(LineVertex) );
        LineVertexArray->SetLineInfo(LineVertexBuffer);

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
        delete[] m_Ptr;
        delete[] m_LinePtr;
        delete[] m_TriPtr;

        delete m_ShaderDebug__Square;
        delete m_ShaderDebug__Line;
        delete m_ShaderDebug__Triangle;
    };

    void Renderer2D::DrawQuad( const glm::vec2& pos, const glm::vec2& size, glm::vec4 color) {
        glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), glm::vec3( pos.x, pos.y, 0.0f ) ) * glm::scale( glm::mat4( 1.0f ), glm::vec3( size.x, size.y, 1.0f ) );
        DrawQuad(transform, color);
    };

    void Renderer2D::DrawQuadRotated(const glm::vec2& pos, const glm::vec2& size, float angle, glm::vec4 color) {
        glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), glm::vec3( pos.x, pos.y, 0.0f ) ) * glm::rotate( glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale( glm::mat4( 1.0f ), glm::vec3( size.x, size.y, 1.0f ) );
        DrawQuad( transform, color );
    };

    void Renderer2D::DrawTriangle( const glm::mat4& transform, glm::vec4 color) {
        if( m_TriangleCount >= m_MaxTri ) {
            Flush();
            BeginBatch();
        };

        for( int i = 0; i < 3; i++ ) {
            m_TriPtrBuff->position = transform * triangle_transforms[ i ];
            m_TriPtrBuff->color = color;
            m_TriPtrBuff++;
        };

        m_TriangleCount += 3;
    };

    void Renderer2D::DrawTriangle( const glm::vec2& position, const glm::vec2& size, const float angle, glm::vec4 color) {
        glm::mat4 transform = glm::translate( glm::mat4(1.0f), glm::vec3( position.x, position.y, 0.0f) ) * glm::rotate( glm::mat4( 1.0f ), glm::radians( angle ), glm::vec3( 0.0f, 0.0f, 1.0f ) ) * glm::scale( glm::mat4( 1.0f ), glm::vec3( size.x, size.y, 1.0f ) );
        DrawTriangle(transform, color);
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
		glm::vec3 p0 = glm::vec3(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, 0.0f);
		glm::vec3 p1 = glm::vec3(pos.x + size.x * 0.5f, pos.y - size.y * 0.5f, 0.0f);
		glm::vec3 p2 = glm::vec3(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, 0.0f);
		glm::vec3 p3 = glm::vec3(pos.x - size.x * 0.5f, pos.y + size.y * 0.5f, 0.0f);

		DrawLine(p0, p1, color );
		DrawLine(p1, p2, color );
		DrawLine(p2, p3, color );
		DrawLine(p3, p0, color );
    };


    void Renderer2D::Flush() {

        // Render all the quads.
        if( m_IndexCount ) {
            const auto& size_ptr = ( uint8_t* )m_PtrBuff - ( uint8_t* )m_Ptr;
            QuadVertexBuffer->SetData(m_Ptr, size_ptr);
            
            for (uint32_t i = 0; i < m_TextureSlotIndex; i++)
		    	m_TextureSlots[i]->Bind(i);

            m_ShaderDebug__Square->Use();
            QuadVertexArray->Bind();
            const uint32_t index_count = m_IndexCount ? m_IndexCount : QuadVertexArray->GetIndexBuffer()->GetCount();
            glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, nullptr );
        };

        //// Render all the lines.
        if( m_LineCount ) {
            const auto& size_ptr = ( uint8_t* )m_LinePtrBuff - ( uint8_t* )m_LinePtr;
            LineVertexBuffer->SetData(m_LinePtr, size_ptr);
           
            m_ShaderDebug__Line->Use();
            LineVertexArray->Bind();
            glDrawArrays(GL_LINES, 0, m_LineCount);
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

        m_TriangleCount = 0;
        m_TriPtrBuff = m_TriPtr;

        m_TextureSlotIndex = 1;
    };

    /* onödig funktion: tabort. */
    void Renderer2D::EndBatch() { };

    void Renderer2D::Begin( const Renderer::OrthoGraphicCamera& camera ) {
        render_camera = camera;
        std::memset(&stats, 0, sizeof( Stats ) );
        glm::mat4 view_projection = render_camera.GetViewProjectionMatrix() * glm::inverse(glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ) ) );
        m_ShaderDebug__Square->SetUniformMat4("u_ViewProjection", view_projection);
        m_ShaderDebug__Line->SetUniformMat4("u_ViewProjection", view_projection );
        m_ShaderDebug__Triangle->SetUniformMat4("u_ViewProjection", view_projection );

        BeginBatch();
    };

    void Renderer2D::Begin( const glm::mat4& projection, const glm::mat4& transform ) {
        //render_camera = camera;

        std::memset(&stats, 0, sizeof( Stats ) );

        glm::mat4 view_projection = projection * glm::inverse( transform );

        m_ShaderDebug__Square->SetUniformMat4("u_ViewProjection", view_projection );
        m_ShaderDebug__Line->SetUniformMat4("u_ViewProjection", view_projection );
        m_ShaderDebug__Triangle->SetUniformMat4("u_ViewProjection", view_projection );
        
        BeginBatch();
    };

    void Renderer2D::End() {
        Flush();
    };

    void Renderer2D::DrawSprite( const glm::mat4& transform, SpriteRendererComponent& component) {
        if(component.sprite.get())
            DrawTexture(transform, component.sprite, component.tiling, component.color);
        else
            DrawQuad(transform, component.color);            
    };
};