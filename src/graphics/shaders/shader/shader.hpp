#pragma once
#include <iostream>
#include <string>
#include <glfw/glfw3.h>
#include <glm/mat4x4.hpp>

namespace Viper::Graphics
{
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
        void SetUniformMat4(const std::string& Name, const glm::mat4& Value) const;
        void SetVector3(const std::string &Name, const glm::vec3 &Value) const;

        inline unsigned int ID() const { return ProgramID; }
    private:
        unsigned int ProgramID;
        static void CheckCompileErrors(unsigned int Shader, const std::string& Type);


    };
}