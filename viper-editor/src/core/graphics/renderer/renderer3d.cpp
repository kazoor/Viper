#define OBJL_CONSOLE_OUTPUT
#include <glad/glad.h>
#include <cstring>
#include <string>
#include "renderer3d.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include "objloader.hpp"


struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normals;
};

constexpr uint32_t max_quads = 20000;
constexpr uint32_t max_vertices = max_quads * 24;
constexpr uint32_t max_indices = max_quads * 36;

void DumpFileData( const char* file_to_dump ) {
    objl::Loader Loader;

	// Load .obj File
	bool loadout = Loader.LoadFile(file_to_dump);

	// Check to see if it loaded

	// If so continue
	if (loadout)
	{
		// Create/Open e1Out.txt
		std::ofstream file("object_file_dump.txt");

		// Go through each loaded mesh and out its contents
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];

			// Print Mesh Name
			file << "Mesh " << i << ": " << curMesh.MeshName << "\n";

			// Print Vertices
			file << "Vertices:\n";

			// Go through each vertex and print its number,
			//  position, normal, and texture coordinate
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				file << "glm::vec4(" << curMesh.Vertices[j].Position.X << "f, " << curMesh.Vertices[j].Position.Y << "f, " << curMesh.Vertices[j].Position.Z << "f, 1.0f)," << std::endl;
			}

            for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				file << "glm::vec3(" << curMesh.Vertices[j].Normal.X << ", " << curMesh.Vertices[j].Normal.Y << ", " << curMesh.Vertices[j].Normal.Z << "), " << std::endl;
			}

			// Print Indices
			file << "Indices:\n";

			// Go through every 3rd index and print the
			//	triangle that these indices represent
			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				file << curMesh.Indices[j] << ", " << curMesh.Indices[j + 1] << ", " << curMesh.Indices[j + 2] << ", " << std::endl;
			}

			file << "\n";
		}

		// Close File
		file.close();
	}
}

void CorrectIndex(uint32_t* indices, size_t indices_count, size_t vertices_count) {
    std::ofstream load_data("indices_corrected.log");
    for( uint32_t i = 0; i < indices_count; i++ ) {
        load_data << "m_indices[ i + " << i << " ] = offset + " << indices[i] << ";" << std::endl;
    };

    load_data << "i += " << indices_count << "." << std::endl;
    load_data << "offset += " << vertices_count << "." << std::endl;

    load_data << "#define VIPER_INDICE_COUNT " << indices_count << std::endl;
    load_data << "#define VIPER_VERICE_COUNT " << vertices_count << std::endl;

    load_data << "Next object would be located at " << vertices_count << std::endl;
    load_data.close();
};

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
        }

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
    uint32_t m_Vao = 0;
    uint32_t m_Vbo = 0;
    uint32_t m_Ebo = 0;
    uint32_t m_IndiceCount = 0;
    uint32_t m_Cube = 0;
    Renderer3D::Stats3D_t stats3d;
    Graphics::Shader* m_Shader = nullptr;

    Vertex* Ptr = nullptr;
    Vertex* PtrBuffer = nullptr;

    constexpr glm::vec4 cube_object_transform[] = {
        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f),
        glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f),
        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f),
        glm::vec4(0.5f, 0.5f, -0.5f, 1.0f),
        glm::vec4(0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4(0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4(0.5f, 0.5f, -0.5f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
        glm::vec4(0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4(0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f),
        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4(0.5f, -0.5f, -0.5f, 1.0f),
        glm::vec4(0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4(-0.5f, -0.5f, 0.5f, 1.0f),
        glm::vec4(0.5f, 0.5f, -0.5f, 1.0f),
        glm::vec4(-0.5f, 0.5f, -0.5f, 1.0f),
        glm::vec4(-0.5f, 0.5f, 0.5f, 1.0f),
        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)
    };

    constexpr glm::vec3 cube_object_normals[] = {
        glm::vec3(-1, 0, 0), 
        glm::vec3(-1, 0, 0), 
        glm::vec3(-1, 0, 0), 
        glm::vec3(-1, 0, 0), 
        glm::vec3(0, 0, -1), 
        glm::vec3(0, 0, -1), 
        glm::vec3(0, 0, -1), 
        glm::vec3(0, 0, -1), 
        glm::vec3(1, 0, 0), 
        glm::vec3(1, 0, 0), 
        glm::vec3(1, 0, 0), 
        glm::vec3(1, 0, 0), 
        glm::vec3(0, 0, 1), 
        glm::vec3(0, 0, 1), 
        glm::vec3(0, 0, 1), 
        glm::vec3(0, 0, 1), 
        glm::vec3(0, -1, 0), 
        glm::vec3(0, -1, 0), 
        glm::vec3(0, -1, 0), 
        glm::vec3(0, -1, 0), 
        glm::vec3(0, 1, 0), 
        glm::vec3(0, 1, 0), 
        glm::vec3(0, 1, 0), 
        glm::vec3(0, 1, 0)
    };

    Ref< VertexArray > QuadArray;
    Ref< VertexBuffer > QuadBuffer;

    void Renderer3D::Init() {
        m_Shader = new Graphics::Shader("resources/shaders/Renderer3D_Quad.vert", "resources/shaders/Renderer3D_Quad.frag");
        Ptr = new Vertex[ max_vertices ];

        QuadArray = VertexArray::Create();
        QuadBuffer = VertexBuffer::Create(max_vertices * sizeof(Vertex));
        QuadArray->SetQuad(QuadBuffer);

        uint32_t* m_indices = new uint32_t[ max_indices ];
        uint32_t offset = 0;
        for( uint32_t i = 0; i < max_indices; i += 36 ) {
            m_indices[ i + 0 ] = offset + 0;
            m_indices[ i + 1 ] = offset + 1;
            m_indices[ i + 2 ] = offset + 3;
            m_indices[ i + 3 ] = offset + 1;
            m_indices[ i + 4 ] = offset + 2;
            m_indices[ i + 5 ] = offset + 3;
            m_indices[ i + 6 ] = offset + 4;
            m_indices[ i + 7 ] = offset + 5;
            m_indices[ i + 8 ] = offset + 7;
            m_indices[ i + 9 ] = offset + 5;
            m_indices[ i + 10 ] = offset + 6;
            m_indices[ i + 11 ] = offset + 7;
            m_indices[ i + 12 ] = offset + 8;
            m_indices[ i + 13 ] = offset + 9;
            m_indices[ i + 14 ] = offset + 11;
            m_indices[ i + 15 ] = offset + 9;
            m_indices[ i + 16 ] = offset + 10;
            m_indices[ i + 17 ] = offset + 11;
            m_indices[ i + 18 ] = offset + 12;
            m_indices[ i + 19 ] = offset + 13;
            m_indices[ i + 20 ] = offset + 15;
            m_indices[ i + 21 ] = offset + 13;
            m_indices[ i + 22 ] = offset + 14;
            m_indices[ i + 23 ] = offset + 15;
            m_indices[ i + 24 ] = offset + 16;
            m_indices[ i + 25 ] = offset + 17;
            m_indices[ i + 26 ] = offset + 19;
            m_indices[ i + 27 ] = offset + 17;
            m_indices[ i + 28 ] = offset + 18;
            m_indices[ i + 29 ] = offset + 19;
            m_indices[ i + 30 ] = offset + 20;
            m_indices[ i + 31 ] = offset + 21;
            m_indices[ i + 32 ] = offset + 23;
            m_indices[ i + 33 ] = offset + 21;
            m_indices[ i + 34 ] = offset + 22;
            m_indices[ i + 35 ] = offset + 23;

            offset += 24;
        };

        Ref<IndexBuffer> QuadIndex = IndexBuffer::Create(m_indices, max_indices);
        QuadArray->SetIndexInfo(QuadIndex);
		delete[] m_indices;
    };

    void Renderer3D::Shutdown() {
        delete[] Ptr;
        delete m_Shader;
        glDeleteVertexArrays(1, &m_Vao);
        glDeleteBuffers(1, &m_Vbo);
        glDeleteBuffers(1, &m_Ebo);
    };

    void Renderer3D::Quad( const glm::mat4& transform, const glm::vec4& color ) {
        if( m_IndiceCount >= max_indices ) {
            Flush();
            BeginBatch();
        };
        
        for( int index = 0; index < 24; index++ ) {
            PtrBuffer->position = transform * cube_object_transform[ index ];
            PtrBuffer->color = color;
            PtrBuffer->normals = cube_object_normals[index];
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

        BeginBatch();
    };

    void Renderer3D::End() {
        Flush();
    };

    void Renderer3D::Flush() {
        if( m_IndiceCount ) {
            const auto& size_ptr = ( uint8_t* )PtrBuffer - ( uint8_t* )Ptr;
            QuadBuffer->SetData(Ptr, size_ptr);

            m_Shader->Use();
            QuadArray->Bind();
            glDrawElements(GL_TRIANGLES, m_IndiceCount, GL_UNSIGNED_INT, nullptr);
        };

        stats3d.m_IndexCount = m_IndiceCount;
        stats3d.m_CubeCount = m_Cube;
        stats3d.m_DrawCalls++;
    };

    void Renderer3D::SetLightPosition( const glm::vec3& pos, const glm::vec4& color, float intensity ) {
        m_Shader->SetVector3("lightPosition", pos);
        m_Shader->SetVector3("lightColor", color);
        m_Shader->SetFloat("lightIntensity", intensity);
    };

    void Renderer3D::BeginBatch() {
        m_IndiceCount = 0;
        m_Cube = 0;
        PtrBuffer = Ptr;
    };
};