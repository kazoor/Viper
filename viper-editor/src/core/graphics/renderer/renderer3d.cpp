#define OBJL_CONSOLE_OUTPUT
#include <glad/glad.h>
#include <cstring>
#include <string>
#include "renderer3d.hpp"
#include "presets3d/cube.hpp"


struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normals;
};

struct VertexLight {
    glm::vec3 position;
    glm::vec4 color;
};

constexpr uint32_t max_quads = 20000;
constexpr uint32_t max_vertices = max_quads * VIPER_Cube_Cube_001_VERT_SIZE;
constexpr uint32_t max_indices = max_quads * VIPER_Cube_Cube_001_IND_SIZE;

// flytta mig till en separat fil.
namespace Viper {
    class IndexBuffer {
    public:
        IndexBuffer(uint32_t* data, size_t size) : m_Count( size ) {
            glCreateBuffers(1, &m_Ebo);

            glBindBuffer(GL_ARRAY_BUFFER, m_Ebo);

            glBufferData(GL_ARRAY_BUFFER, size * sizeof( uint32_t ), data, GL_STATIC_DRAW);
        };

        ~IndexBuffer() {
            glDeleteBuffers(1, &m_Ebo);
        };

        void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo); };

        void Unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); };

        uint32_t GetCount() const { return m_Count; };

        static Ref< IndexBuffer > Create( uint32_t* data, size_t size ) {
            return CreateRef< IndexBuffer >( data, size );
        };
    private:
        uint32_t m_Ebo;
        uint32_t m_Count;
    };

    class VertexBuffer {
    public:
        VertexBuffer(const uint32_t size) {
            glCreateBuffers(1, &m_Vbo);
            Bind();

            glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
        };

         VertexBuffer( float* vertices, const uint32_t maxbuffer ) {
            glCreateBuffers(1, &m_Vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo );
            glBufferData(GL_ARRAY_BUFFER, maxbuffer, vertices, GL_STATIC_DRAW );
        };

        ~VertexBuffer() {
            glDeleteBuffers(1, &m_Vbo);
        };

        void Bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_Vbo); };

        void Unind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); };

        void SetData(const void* ptr, uint32_t size ) {
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, ptr );
        };

        static Ref< VertexBuffer > Create(const uint32_t size) {
            return CreateRef< VertexBuffer >( size );
        };

        static Ref< VertexBuffer > Create(float* vertices, const uint32_t maxbuffer) {
            return CreateRef< VertexBuffer >( vertices, maxbuffer );
        };
    private:
        uint32_t m_Vbo;  
    };

    class VertexArray {
    public:
        VertexArray() {
            glCreateVertexArrays(1, &m_Vao);
        };

        ~VertexArray() {
            glDeleteVertexArrays(1, &m_Vao);
        };

        void SetIndexInfo( const Ref< IndexBuffer >& indexbuffer ) {
            glBindVertexArray(m_Vao);
            indexbuffer->Bind();

            m_indexbuffer = indexbuffer;
        };

        void SetQuad(const Ref< VertexBuffer >& vertexbuffer) {
            Bind();
            vertexbuffer->Bind();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof(Vertex, position));
    
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof(Vertex, color));
    
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof(Vertex, normals));
        };

        // light doesnt need normals iirc.
        void SetLight(const Ref< VertexBuffer >& vertexbuffer) {
            Bind();
            vertexbuffer->Bind();
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( VertexLight ), ( const void* )offsetof(VertexLight, position));
    
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( VertexLight ), ( const void* )offsetof(VertexLight, color));
        };

        Ref< IndexBuffer > GetIndexBuffer() const { return m_indexbuffer; };

        void Bind() const { glBindVertexArray(m_Vao); };

        void Unbind() const { glBindVertexArray(0); };

        static Ref< VertexArray > Create( ) {
            return CreateRef< VertexArray >( );
        };
    private:
        uint32_t m_Vao;

        Ref< IndexBuffer > m_indexbuffer;
    };
};

// !!

namespace Viper {
    uint32_t m_IndiceCount = 0;
    uint32_t m_LightCount = 0;
    Renderer3D::Stats3D_t stats3d;
    Graphics::Shader* m_Shader = nullptr, *m_ShaderLight = nullptr;

    Vertex* CubePtr = nullptr;
    Vertex* CubePtrBuffer = nullptr;

    VertexLight* LightPtr = nullptr;
    VertexLight* LightPtrBuffer = nullptr;

    Ref< VertexArray > CubeArray;
    Ref< VertexBuffer > CubeBuffer;

    Ref< VertexArray > LightArray;
    Ref< VertexBuffer > LightBuffer;

    void Renderer3D::Init() {
        m_Shader = new Graphics::Shader("resources/shaders/Renderer3D_Quad.vert", "resources/shaders/Renderer3D_Quad.frag");
        m_ShaderLight = new Graphics::Shader("resources/shaders/Renderer3D_Light.vert", "resources/shaders/Renderer3D_Light.frag");
        CubePtr = new Vertex[ max_vertices ];
        LightPtr = new VertexLight[ max_vertices ];

        CubeArray = VertexArray::Create();
        CubeBuffer = VertexBuffer::Create(max_vertices * sizeof(Vertex));
        CubeArray->SetQuad(CubeBuffer);

        uint32_t* m_cube_indices = new uint32_t[ max_indices ];
        uint32_t offset = 0;
        for( uint32_t i = 0; i < max_indices; i += 36) {
        	m_cube_indices[ i + 0 ] = offset + 0;
        	m_cube_indices[ i + 1 ] = offset + 1;
        	m_cube_indices[ i + 2 ] = offset + 3;
        	m_cube_indices[ i + 3 ] = offset + 1;
        	m_cube_indices[ i + 4 ] = offset + 2;
        	m_cube_indices[ i + 5 ] = offset + 3;
        	m_cube_indices[ i + 6 ] = offset + 4;
        	m_cube_indices[ i + 7 ] = offset + 5;
        	m_cube_indices[ i + 8 ] = offset + 7;
        	m_cube_indices[ i + 9 ] = offset + 5;
        	m_cube_indices[ i + 10 ] = offset + 6;
        	m_cube_indices[ i + 11 ] = offset + 7;
        	m_cube_indices[ i + 12 ] = offset + 8;
        	m_cube_indices[ i + 13 ] = offset + 9;
        	m_cube_indices[ i + 14 ] = offset + 11;
        	m_cube_indices[ i + 15 ] = offset + 9;
        	m_cube_indices[ i + 16 ] = offset + 10;
        	m_cube_indices[ i + 17 ] = offset + 11;
        	m_cube_indices[ i + 18 ] = offset + 12;
        	m_cube_indices[ i + 19 ] = offset + 13;
        	m_cube_indices[ i + 20 ] = offset + 15;
        	m_cube_indices[ i + 21 ] = offset + 13;
        	m_cube_indices[ i + 22 ] = offset + 14;
        	m_cube_indices[ i + 23 ] = offset + 15;
        	m_cube_indices[ i + 24 ] = offset + 16;
        	m_cube_indices[ i + 25 ] = offset + 17;
        	m_cube_indices[ i + 26 ] = offset + 19;
        	m_cube_indices[ i + 27 ] = offset + 17;
        	m_cube_indices[ i + 28 ] = offset + 18;
        	m_cube_indices[ i + 29 ] = offset + 19;
        	m_cube_indices[ i + 30 ] = offset + 20;
        	m_cube_indices[ i + 31 ] = offset + 21;
        	m_cube_indices[ i + 32 ] = offset + 23;
        	m_cube_indices[ i + 33 ] = offset + 21;
        	m_cube_indices[ i + 34 ] = offset + 22;
        	m_cube_indices[ i + 35 ] = offset + 23;

        	offset += 24;
        };

        Ref<IndexBuffer> CubeIndex = IndexBuffer::Create(m_cube_indices, max_indices);
        CubeArray->SetIndexInfo(CubeIndex);

        // Light

        LightArray = VertexArray::Create();
        LightBuffer = VertexBuffer::Create(max_vertices * sizeof(VertexLight));
        LightArray->SetLight(LightBuffer);

        // viking note: det finns ingen anledning att skapa en helt ny uint32 pointer array
        // eftersom för light's så vill vi använda samma kub igen.
        // använd den som existerar.
        
        Ref<IndexBuffer> LightIndex = IndexBuffer::Create(m_cube_indices, max_indices);
        LightArray->SetIndexInfo(LightIndex);
		delete[] m_cube_indices;
    };

    void Renderer3D::Shutdown() {
        delete[] CubePtr;
        delete[] LightPtr;
        delete m_Shader;
        delete m_ShaderLight;
    };

    // ah yes, the giga chad cube.
    // has vertices.
    // has colors
    // and has normals
    void Renderer3D::Quad( const glm::mat4& transform, const glm::vec4& color ) {
        if( m_IndiceCount >= max_indices ) {
            Flush();
            BeginBatch();
        };
        
        for( int index = 0; index < VIPER_Cube_Cube_001_VERT_SIZE; index++ ) {
            CubePtrBuffer->position = transform * object_transform[ index ];
            CubePtrBuffer->color = color;
            CubePtrBuffer->normals = object_normals[index];
            CubePtrBuffer++;
        };
        m_IndiceCount += VIPER_Cube_Cube_001_IND_SIZE;
        stats3d.m_CubeCount++;
    };

    Renderer3D::Stats3D_t& Renderer3D::GetStats() { return stats3d; };

    void Renderer3D::Begin(const glm::mat4& camera_transform, const glm::mat4& transform ) {
        std::memset(&stats3d, 0, sizeof( Stats3D_t ) );
        glm::mat4 view_projection = camera_transform * glm::inverse( transform );

        m_Shader->SetUniformMat4("viewmatrix", view_projection );
        m_Shader->Use();
        
        m_ShaderLight->SetUniformMat4("viewmatrix", view_projection );
        m_ShaderLight->Use();
        BeginBatch();
    };

    void Renderer3D::End() {
        Flush();
    };

    void Renderer3D::Flush() {
        if( m_IndiceCount ) {
            const auto& size_ptr = ( uint8_t* )CubePtrBuffer - ( uint8_t* )CubePtr;
            CubeBuffer->SetData(CubePtr, size_ptr);

            CubeArray->Bind();
            glDrawElements(GL_TRIANGLES, m_IndiceCount, GL_UNSIGNED_INT, nullptr);
        };

        if( m_LightCount ) {
            const auto& size_ptr = ( uint8_t* )LightPtrBuffer - ( uint8_t* )LightPtr;
            LightBuffer->SetData(LightPtr, size_ptr);

            LightArray->Bind();
            glDrawElements(GL_TRIANGLES, m_LightCount, GL_UNSIGNED_INT, nullptr);
        };

        stats3d.m_IndexCount = m_IndiceCount + m_LightCount;
        stats3d.m_DrawCalls++;
    };

    // ah yes, the virgin light
    // has vertices
    // has colors
    // doesnt have normals.
    void Renderer3D::SetLightPosition( const glm::mat4& transform, const glm::vec4& color, float intensity ) {
        if( m_LightCount >= max_indices ) {
            Flush();
            BeginBatch();
        };
        
        for( int index = 0; index < VIPER_Cube_Cube_001_VERT_SIZE; index++ ) {
            LightPtrBuffer->position = transform * object_transform[ index ];
            LightPtrBuffer->color = color;
            LightPtrBuffer++;
        };
        m_LightCount += VIPER_Cube_Cube_001_IND_SIZE;
        stats3d.m_CubeCount++;
        stats3d.m_LightSources++;
    };

    void Renderer3D::BeginBatch() {
        m_IndiceCount = 0;
        CubePtrBuffer = CubePtr;

        m_LightCount = 0;
        LightPtrBuffer = LightPtr;
    };
};