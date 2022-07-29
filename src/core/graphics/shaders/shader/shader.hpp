#pragma once
#include <iostream>
#include <string>
#include <glfw/glfw3.h>
#include <glm/mat4x4.hpp>
#include <util/ref/reference.hpp>

namespace Viper::Graphics
{
    class ShaderCommand {
    public:
        static void UploadMat4( uint32_t ProgramID, const std::string& Name, const glm::mat4& Value );

        static void UploadVec2( uint32_t ProgramID, const std::string& Name, const glm::vec2& Value );
        static void UploadVec3( uint32_t ProgramID, const std::string& Name, const glm::vec3& Value );
        static void UploadVec4( uint32_t ProgramID, const std::string& Name, const glm::vec4& Value );

        static void UploadInt( uint32_t ProgramID, const std::string& Name, int Value );
        static void UploadFloat( uint32_t ProgramID, const std::string& Name, float Value );
        static void UploadBool( uint32_t ProgramID, const std::string& Name, bool Value );
        static void UploadDouble( uint32_t ProgramID, const std::string& Name, double Value );
    };

    class Shader
    {
    public:

        // constructor reads and builds the shader
        Shader(const std::string &VertexPath, const std::string &FragmentPath);
        // use/activate the shader
        void Use() const;
        // utility uniform functions
        void SetBool(const std::string &Name, bool Value) const;
        void SetInt(const std::string &Name, int Value) const;
        void SetFloat(const std::string &Name, float Value) const;
        void SetDouble(const std::string &Name, double Value) const;
        void SetUniformMat4(const std::string& Name, const glm::mat4& Value) const;
        void SetVector2(const std::string &Name, const glm::vec2 &Value) const;
        void SetVector3(const std::string &Name, const glm::vec3 &Value) const;
        void SetVector4(const std::string &Name, const glm::vec4 &Value) const;

        inline unsigned int ID() const { return ProgramID; }

        static Ref< Shader > Create( const std::string &VertexPath, const std::string &FragmentPath );
    private:
        unsigned int ProgramID;
        static void CheckCompileErrors(unsigned int Shader, const std::string& Type);
    };
}