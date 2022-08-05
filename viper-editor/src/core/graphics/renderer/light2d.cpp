#include <glad/glad.h>
#include <graphics/shaders/shader/shader.hpp>
#include <util/timer/timer.hpp>

#include "light2d.hpp"
struct LightData {
    glm::vec3 position;
    glm::vec4 color;
};

constexpr uint32_t max_lights = 50;
constexpr uint32_t max_verts = max_lights * 4;

LightData* m_LightPtr = nullptr;
LightData* m_LightBufferPtr = nullptr;
uint32_t m_LightCount = 0;

Viper::Graphics::Shader* m_LightShader = nullptr;

constexpr glm::vec4 transforms[ 3U ] = {
    glm::vec4( -0.5f, -0.5f, 0.0f, 1.0f ),
    glm::vec4(  0.5f, -0.5f, 0.0f, 1.0f ),
    glm::vec4(  0.0f,  0.5f, 0.0f, 1.0f )
};

namespace Viper {
    Light2D::Light2D() {
        glCreateVertexArrays(1, &m_LightVAO);
        glBindVertexArray(m_LightVAO);

        glCreateBuffers(1, &m_LightVBO);
        glBindBuffer(GL_ARRAY_BUFFER, m_LightVBO);
        glBufferData(GL_ARRAY_BUFFER, max_verts * sizeof( LightData ), nullptr, GL_DYNAMIC_DRAW );

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof( LightData ), ( const void* )offsetof( LightData, position ) );

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof( LightData ), ( const void* )offsetof( LightData, color ) );

        glBindBuffer( GL_ARRAY_BUFFER, 0 );
        glBindVertexArray( 0 );
        m_LightPtr = new LightData[ max_verts ];

        m_LightShader = new Graphics::Shader("resources/shaders/light.vert", "resources/shaders/light.frag");
    };

    Light2D::~Light2D() {
        glDeleteVertexArrays(1, &m_LightVAO);
        glDeleteBuffers(1, &m_LightVBO);

        delete m_LightShader;
        delete[] m_LightPtr;
    };

    void Light2D::Begin( const Renderer::OrthoGraphicCamera& camera ) {
        m_camera = camera;

        m_LightBufferPtr = m_LightPtr;
        m_LightCount = 0;
    };
    void Light2D::Light( const glm::vec2& position, glm::vec4 color, float angle) {
        glm::mat4 transform = glm::translate( glm::mat4( 1.0f ), glm::vec3( position.x, position.y, 0.0f ) ) 
        * glm::rotate( glm::mat4( 1.0f ), glm::radians( angle ), glm::vec3(0.0f, 0.0f, 1.0f))
        * glm::scale( glm::mat4( 1.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ) );
        for( int i = 0; i < 3; i++ ) {
            m_LightBufferPtr->position = transform * transforms[ i ];
            m_LightBufferPtr->color = color;
            m_LightBufferPtr++;
        };

        //m_LightBufferPtr->position = glm::vec3(2.0f, 2.0f, 0.0f );//transform * transforms[ i ];
        //m_LightBufferPtr->color = color;
        //m_LightBufferPtr++;
//
        //m_LightBufferPtr->position = glm::vec3(position.x, position.y, 0.0f );//transform * transforms[ i ];
        //m_LightBufferPtr->color = color;
        //m_LightBufferPtr++;

        m_LightCount += 3;
    };

    Timer::Timer ts;
    void Light2D::End() {
        if( m_LightCount ) {
            const auto& size_ptr = ( uint8_t* )m_LightBufferPtr - ( uint8_t* )m_LightPtr;
            glBindBuffer(GL_ARRAY_BUFFER, m_LightVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size_ptr, m_LightPtr );
           
            m_LightShader->Use();
            m_LightShader->SetUniformMat4("u_ViewProjection", m_camera.GetViewProjectionMatrix( ) * glm::translate( glm::mat4( 1.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ) ) );

            m_LightShader->SetVector3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
            m_LightShader->SetVector3("lightColor",  glm::vec3(1.0f, 1.0f, 1.0f));
            
            
            m_LightShader->SetFloat("u_Time",ts.Elapsed());

            glBindVertexArray(m_LightVAO);
            glDrawArrays(GL_TRIANGLES, 0, m_LightCount);
        };
    };
};