#include <glad/glad.h>
#include <cstring>
#include "renderer3d.hpp"


struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
};

constexpr uint32_t max_quads = 20000;
constexpr uint32_t max_vertices = max_quads * 24;
constexpr uint32_t max_indices = max_quads * 36;

namespace Viper {
    
    uint32_t m_Vao = 0;
    uint32_t m_Vbo = 0;
    uint32_t m_Ebo = 0;
    uint32_t m_IndiceCount = 0;
    uint32_t m_Cube = 0;
    Renderer3D::Stats3D_t stats3d;
    Graphics::Shader* m_Shader = nullptr, *m_ShaderLight = nullptr;

    Vertex* Ptr = nullptr;
    Vertex* PtrBuffer = nullptr;

    constexpr glm::vec4 quad_cube_transform[] = {
        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4( 0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4( 0.5f,  0.5f, -0.5f, 1.0f),
        glm::vec4(-0.5f,  0.5f, -0.5f, 1.0f),
        glm::vec4(-0.5f, -0.5f,  0.5f, 1.0f),
        glm::vec4( 0.5f, -0.5f,  0.5f, 1.0f),
        glm::vec4( 0.5f,  0.5f,  0.5f, 1.0f),
        glm::vec4(-0.5f,  0.5f,  0.5f, 1.0f)
    };

    void Renderer3D::Init() {
        m_Shader = new Graphics::Shader("resources/shaders/Renderer3D_Quad.vert", "resources/shaders/Renderer3D_Quad.frag");
        m_ShaderLight = new Graphics::Shader("resources/shaders/Renderer3D_Light.vert", "resources/shaders/Renderer3D_Light.frag");
        Ptr = new Vertex[ max_vertices ];

        glCreateVertexArrays(1, &m_Vao);
        glBindVertexArray(m_Vao);

        glCreateBuffers(1, &m_Vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
        glBufferData(GL_ARRAY_BUFFER, max_vertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof(Vertex, position));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( Vertex ), ( const void* )offsetof(Vertex, color));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        uint32_t m_indices[] = {
            0, 1, 3,    3, 1, 2,
            1, 5, 2,    2, 5, 6,
            5, 4, 6,    6, 4, 7,
            4, 0, 7,    7, 0, 3,
            3, 2, 7,    7, 2, 6,
            4, 5, 0,    0, 5, 1
        };

        uint32_t* m_index = new uint32_t[ max_indices ];
        uint32_t offset = 0;

        #define VertexIndex(n_index, n_offset) m_index[ index + n_index ] = offset + n_offset
        for( size_t index = 0; index < max_indices; index += 36 ) {
            VertexIndex(0, 0);
            VertexIndex(1, 1);
            VertexIndex(2, 3); //

            VertexIndex(3, 3);
            VertexIndex(4, 1);
            VertexIndex(5, 2);

            VertexIndex(6, 1);
            VertexIndex(7, 5);
            VertexIndex(8, 2);

            VertexIndex(9, 2);
            VertexIndex(10, 5);
            VertexIndex(11, 6);

            VertexIndex(12, 5);
            VertexIndex(13, 4);
            VertexIndex(14, 6);

            VertexIndex(15, 6);
            VertexIndex(16, 4);
            VertexIndex(17, 7);

            VertexIndex(18, 4);
            VertexIndex(19, 0);
            VertexIndex(20, 7);

            VertexIndex(21, 7);
            VertexIndex(22, 0);
            VertexIndex(23, 3);

            VertexIndex(24, 3);
            VertexIndex(25, 2);
            VertexIndex(26, 7);

            VertexIndex(27, 7);
            VertexIndex(28, 2);
            VertexIndex(29, 6);

            VertexIndex(30, 4);
            VertexIndex(31, 5);
            VertexIndex(32, 0);

            VertexIndex(33, 0);
            VertexIndex(34, 5);
            VertexIndex(35, 1);

            offset += 8;
        };

        glCreateBuffers(1, &m_Ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_indices * sizeof( uint32_t ), m_index, GL_STATIC_DRAW);
        delete[] m_index;
    };

    void Renderer3D::Shutdown() {
        delete[] Ptr;
        delete m_Shader;
        delete m_ShaderLight;
        glDeleteVertexArrays(1, &m_Vao);
        glDeleteBuffers(1, &m_Vbo);
        glDeleteBuffers(1, &m_Ebo);
    };

    void Renderer3D::Quad( const glm::mat4& transform, const glm::vec4& color ) {
        if( m_IndiceCount >= max_indices ) {
            Flush();
            BeginBatch();
        };
        
        for( int index = 0; index < 8; index++ ) {
            PtrBuffer->position = transform * quad_cube_transform[ index ];
            PtrBuffer->color = color;
            PtrBuffer++;
        };
        m_IndiceCount += 36;
        m_Cube++;
        
    };

    Renderer3D::Stats3D_t& Renderer3D::GetStats() { return stats3d; };

    void Renderer3D::Begin(const glm::mat4& camera_transform, const glm::mat4& transform ) {
        std::memset(&stats3d, 0, sizeof( Stats3D_t ) );
        glm::mat4 view_projection = camera_transform * glm::inverse( transform );
        m_Shader->SetUniformMat4("viewmatrix", view_projection );
        m_ShaderLight->SetUniformMat4("viewmatrix", view_projection );

        BeginBatch();
    };

    void Renderer3D::End() {
        Flush();
    };

    void Renderer3D::Flush() {
        if( m_IndiceCount ) {
            const auto& size_ptr = ( uint8_t* )PtrBuffer - ( uint8_t* )Ptr;
            glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size_ptr, Ptr );

            m_Shader->Use();
            glBindVertexArray(m_Vao);
            glDrawElements(GL_TRIANGLES, m_IndiceCount, GL_UNSIGNED_INT, nullptr);
        };

        stats3d.m_IndexCount = m_IndiceCount;
        stats3d.m_CubeCount = m_Cube;
        stats3d.m_DrawCalls++;
    };

    void Renderer3D::BeginBatch() {
        m_IndiceCount = 0;
        m_Cube = 0;
        PtrBuffer = Ptr;
    };

    void Renderer3D::EndBatch() {

    };

    void Renderer3D::Quad() {
        
    };
};