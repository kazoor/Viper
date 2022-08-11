#pragma once
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <util/ref/reference.hpp>
#include <vector>
#include <string>
#include "color.hpp"

namespace Viper {
    enum class ShaderType : int {
	    S_INT, S_FLOAT, S_DOUBLE, S_BOOL,
	    S_VEC2, S_VEC3, S_VEC4
    };

    struct Shader_t {
        ShaderType Type;
        const char* Identifier;
    };

    // <!-- Begin Index Buffer
    class IndexBuffer {
    public:
        IndexBuffer(uint32_t* data, size_t size);

        ~IndexBuffer();

        void Bind() const;

        void Unbind() const;

        uint32_t GetCount() const;

        static Ref< IndexBuffer > Create( uint32_t* data, size_t size );
    private:
        uint32_t m_Ebo;
        uint32_t m_Count;
    };
    // End Index Buffer --!>

    // <!-- Begin Vertex Buffer
    class VertexBuffer {
    public:
        VertexBuffer(const uint32_t size);

        VertexBuffer( float* vertices, const uint32_t maxbuffer );

        ~VertexBuffer();

        void Bind() const;

        void Unind() const;

        void SetData(const void* ptr, uint32_t size );

        static Ref< VertexBuffer > Create(const uint32_t size);

        static Ref< VertexBuffer > Create(float* vertices, const uint32_t maxbuffer);
    private:
        uint32_t m_Vbo;  
    };
    // End Vertex Buffer --!>

    // <!-- Begin Vertex Array
    class VertexArray {
    public:
        VertexArray();

        ~VertexArray();

        void SetIndexInfo( const Ref< IndexBuffer >& indexbuffer );

        template< class TVertex >
        void SetShaderInfo(const Ref< VertexBuffer >& vertexbuffer, const std::vector< Shader_t > sinfo ) {
            Bind();
            vertexbuffer->Bind();

            uint32_t m_current_array = 0;
	        uint32_t m_current_stride = 0;

	        for ( const auto offset : sinfo ) {
                glEnableVertexAttribArray(m_current_array);
	        	uint32_t m_stride_offset = 0;
	        	uint16_t m_stride_type = 0;
	        	uint32_t m_stride_alt = 0;

	        	switch( offset.Type ) {
	        		case ShaderType::S_INT: m_stride_offset = 4; m_stride_alt = 1; m_stride_type = 0x1404; break;
	        		case ShaderType::S_FLOAT: m_stride_offset = 4; m_stride_alt = 1; m_stride_type = 0x1406; break;
	        		case ShaderType::S_DOUBLE: m_stride_offset = 8; m_stride_alt = 1; m_stride_type = 0x140A; break;
	        		case ShaderType::S_BOOL: m_stride_offset = 1; m_stride_alt = 1; m_stride_type = 0x8B56; break;

	        		case ShaderType::S_VEC2: m_stride_offset = 8; m_stride_alt = 2; m_stride_type = 0x1406; break;
	        		case ShaderType::S_VEC3: m_stride_offset = 12; m_stride_alt = 3; m_stride_type = 0x1406; break;
	        		case ShaderType::S_VEC4: m_stride_offset = 16; m_stride_alt = 4; m_stride_type = 0x1406; break;
	        	};
                glVertexAttribPointer(m_current_array, m_stride_alt, m_stride_type, GL_FALSE, sizeof( TVertex ), reinterpret_cast< const void* >( m_current_stride ) );
	        	m_current_array++;
	        	m_current_stride += m_stride_offset;
	        };
        };

        Ref< IndexBuffer > GetIndexBuffer() const;

        void Bind() const;

        void Unbind() const;

        static Ref< VertexArray > Create( );
    private:
        uint32_t m_Vao;

        Ref< IndexBuffer > m_indexbuffer;
    };
    // End Vertex Array --!>

    class UniformBuffer {
    public:
        UniformBuffer( uint32_t size, uint32_t binding );

        ~UniformBuffer();

        void SetData(const void* data, uint32_t size, uint32_t offset = 0);

        static Ref< UniformBuffer > Create(uint32_t size, uint32_t binding);
    private:
        uint32_t m_Ubo;
    };
};