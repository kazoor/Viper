
#include <glad/glad.h>
#include <cstring>
#include <string>
#include "renderer3d.hpp"
#include "presets3d/cube.hpp"
#include "api/vertex.hpp"


//#include "objloader.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normals;
};

struct VertexLight {
    glm::vec3 position;
    glm::vec4 color;
};

struct MeshVertex {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec3 normals;
};

constexpr uint32_t max_quads = 20000;
constexpr uint32_t max_vertices = max_quads * VIPER_Cube_Cube_001_VERT_SIZE;
constexpr uint32_t max_indices = max_quads * VIPER_Cube_Cube_001_IND_SIZE;


namespace Viper {
    uint32_t m_IndiceCount = 0;
    uint32_t m_LightCount = 0;
    uint32_t m_MeshCount = 0;

    Renderer3D::Stats3D_t stats3d;

    Ref< Graphics::Shader > CubeShader;
    Ref< Graphics::Shader > LightShader;

    Vertex* CubePtr = nullptr;
    Vertex* CubePtrBuffer = nullptr;

    VertexLight* LightPtr = nullptr;
    VertexLight* LightPtrBuffer = nullptr;

    MeshVertex* MeshPtr = nullptr;
    MeshVertex* MeshPtrBuffer = nullptr;

    Ref< VertexArray > CubeArray;
    Ref< VertexBuffer > CubeBuffer;

    Ref< VertexArray > LightArray;
    Ref< VertexBuffer > LightBuffer;

    Ref< VertexArray > MeshArray;
    Ref< VertexBuffer > MeshBuffer;

    struct CameraData {
		glm::mat4 ViewProjection;
	};

	CameraData CameraBuffer;

	Ref<UniformBuffer> CameraUniformBuffer;

    static void DrawIndexed( const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
        vertexArray->Bind();
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    };

    Mesh* LoadMeshFromPath( const char* path ) {
        Mesh* m = new Mesh();
        std::string inputfile = path;
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = "./"; // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(inputfile, reader_config)) {
          if (!reader.Error().empty()) {
              std::cerr << "TinyObjReader: " << reader.Error();
          }
          exit(1);
        }

        if (!reader.Warning().empty()) {
          std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            for (size_t f = 0; f < shapes[s].mesh.indices.size() / 3; f++) {
                // Get the three indexes of the face (all faces are triangular)
                tinyobj::index_t idx0 = shapes[s].mesh.indices[3 * f + 0];
                tinyobj::index_t idx1 = shapes[s].mesh.indices[3 * f + 1];
                tinyobj::index_t idx2 = shapes[s].mesh.indices[3 * f + 2];
                m->indices.push_back(idx0.vertex_index);
                m->indices.push_back(idx1.vertex_index);
                m->indices.push_back(idx2.vertex_index);
            }
          // Loop over faces(polygon)
          size_t index_offset = 0;
          for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
              // access to vertex
              tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
              tinyobj::real_t vx = attrib.vertices[3*size_t(idx.vertex_index)+0];
              tinyobj::real_t vy = attrib.vertices[3*size_t(idx.vertex_index)+1];
              tinyobj::real_t vz = attrib.vertices[3*size_t(idx.vertex_index)+2];

              m->transform.push_back(glm::vec4(vx, vy, vz, 1.0f));

              // Check if `normal_index` is zero or positive. negative = no normal data
              if (idx.normal_index >= 0) {
                tinyobj::real_t nx = attrib.normals[3*size_t(idx.normal_index)+0];
                tinyobj::real_t ny = attrib.normals[3*size_t(idx.normal_index)+1];
                tinyobj::real_t nz = attrib.normals[3*size_t(idx.normal_index)+2];
                m->normals.push_back(glm::vec3(nx, ny, nz));
              }

              // Check if `texcoord_index` is zero or positive. negative = no texcoord data
              if (idx.texcoord_index >= 0) {
                tinyobj::real_t tx = attrib.texcoords[2*size_t(idx.texcoord_index)+0];
                tinyobj::real_t ty = attrib.texcoords[2*size_t(idx.texcoord_index)+1];
              }

              // Optional: vertex colors
              tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
              tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
              tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
              m->colors.push_back(glm::vec3(red, green, blue));
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
          }
        }
        return m;
    };
    
    void FreeMesh(Mesh* m) {
        delete m;
    };
    

    void Renderer3D::Init() {
        CubeShader = Graphics::Shader::Create("resources/shaders/Renderer3D_Quad.vert", "resources/shaders/Renderer3D_Quad.frag");
        LightShader = Graphics::Shader::Create("resources/shaders/Renderer3D_Light.vert", "resources/shaders/Renderer3D_Light.frag");

        CubePtr = new Vertex[ max_vertices ];
        LightPtr = new VertexLight[ max_vertices ];
        MeshPtr = new MeshVertex[ max_vertices ];

        CubeArray = VertexArray::Create();
        CubeBuffer = VertexBuffer::Create(max_vertices * sizeof(Vertex));

        // CubeArray->SetShaderInfo: Detta vore ytterst dumt att göra.
        // Vi vill inte sätta VertexBuffer attributer i en VertexArray, istället vill vi sätta det i
        // en vertexbuffer.
        CubeArray->SetShaderInfo< Vertex >( CubeBuffer, {
            { ShaderType::S_VEC3, "m_Position" }, // Position
            { ShaderType::S_VEC4, "m_Color" }, // Color
            { ShaderType::S_VEC3, "m_Normals" } // Normals
        });

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
		delete[] m_cube_indices;

        // Light

        LightArray = VertexArray::Create();
        LightBuffer = VertexBuffer::Create(max_vertices * sizeof(VertexLight));
        LightArray->SetShaderInfo< VertexLight >(LightBuffer,{
            { ShaderType::S_VEC3, "m_Position" }, // Position
            { ShaderType::S_VEC4, "m_Color" } // Color
        });
        LightArray->SetIndexInfo(CubeIndex);

        CameraUniformBuffer = UniformBuffer::Create(sizeof(CameraData), 0);
    };

    void Renderer3D::Shutdown() {
        delete[] CubePtr;
        delete[] LightPtr;
        delete[] MeshPtr;
    };

    void Renderer3D::InvalidateMesh(Mesh* mesh) {
        if( !mesh->mesh_is_constructed ) {
            MeshArray = VertexArray::Create();
            MeshBuffer = VertexBuffer::Create(mesh->transform.size() * sizeof(MeshVertex));
            MeshArray->SetShaderInfo< MeshVertex >(MeshBuffer,{
                { ShaderType::S_VEC3, "m_Position" }, // Position
                { ShaderType::S_VEC4, "m_Color" }, // Color,
                { ShaderType::S_VEC3, "m_Normals"} // Normals
            });

            Ref<IndexBuffer> MeshIndex = IndexBuffer::Create(&mesh->indices[0], mesh->indices.size());
            MeshArray->SetIndexInfo(MeshIndex);

            mesh->mesh_is_constructed = true;
        };
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

        CameraBuffer.ViewProjection = view_projection;
        CameraUniformBuffer->SetData(&CameraBuffer, sizeof(CameraData));


        BeginBatch();
    };

    void Renderer3D::End() {
        Flush();
    };

    void Renderer3D::Flush() {

        if( m_IndiceCount ) {
            const auto& size_ptr = ( uint8_t* )CubePtrBuffer - ( uint8_t* )CubePtr;
            CubeBuffer->SetData(CubePtr, size_ptr);

            CubeShader->Use();
            DrawIndexed(CubeArray, m_IndiceCount);
        };

        if( m_LightCount ) {
            const auto& size_ptr = ( uint8_t* )LightPtrBuffer - ( uint8_t* )LightPtr;
            LightBuffer->SetData(LightPtr, size_ptr);

            LightShader->Use();
            DrawIndexed(LightArray, m_LightCount);
        };

        if( m_MeshCount ) {
            const auto& size_ptr = ( uint8_t* )MeshPtrBuffer - ( uint8_t* )MeshPtr;
            MeshBuffer->SetData(MeshPtr, size_ptr);
            
            CubeShader->Use();
            //DrawIndexed(MeshArray, m_MeshCount);
            glDrawArrays(GL_TRIANGLES, 0, m_MeshCount);
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

    void Renderer3D::SetLightPosition( const glm::mat4& transform, const glm::vec3& pos, const glm::vec4& color, float intensity ) {
        if( m_LightCount >= max_indices ) {
            Flush();
            BeginBatch();
        };
        
        for( int index = 0; index < VIPER_Cube_Cube_001_VERT_SIZE; index++ ) {
            LightPtrBuffer->position = transform * object_transform[ index ];
            LightPtrBuffer->color = color;
            LightPtrBuffer++;
        };
        //CubeShader->SetVector3("light_positions", pos);


        m_LightCount += VIPER_Cube_Cube_001_IND_SIZE;
        stats3d.m_CubeCount++;
        stats3d.m_LightSources++;

    };

    void Renderer3D::DrawMesh( const glm::mat4& transform, Mesh* mesh ) {
        for( int i = 0; i < mesh->transform.size(); i++ ) {
            MeshPtrBuffer->position = transform * mesh->transform[ i ];
            MeshPtrBuffer->color = glm::vec4(mesh->colors[ i ], 1.0f);
            MeshPtrBuffer->normals = mesh->normals[ i ];
            MeshPtrBuffer++;
        };
        m_MeshCount += mesh->indices.size();
    };

    void Renderer3D::BeginBatch() {
        
        m_IndiceCount = 0;
        CubePtrBuffer = CubePtr;

        m_LightCount = 0;
        LightPtrBuffer = LightPtr;

        m_MeshCount = 0;
        MeshPtrBuffer = MeshPtr;
    };
};