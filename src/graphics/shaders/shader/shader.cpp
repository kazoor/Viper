// Copyright (c) 2022.

#include <glad/glad.h>
#include <sstream>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "../../../util/filehandler/filehandler.hpp"
#include "../../../util/globals/global.hpp"

namespace Viper::Graphics {
    Shader::Shader(const std::string &VertexPath, const std::string &FragmentPath) {

        std::string vertexCode = Viper::Util::FileHandler(VertexPath).ReadFile();
        std::string fragmentCode = Viper::Util::FileHandler(FragmentPath).ReadFile();

        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        CheckCompileErrors(vertex, "VERTEX");

        // Fragment Shader.
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        CheckCompileErrors(fragment, "FRAGMENT");

        // Shader Program.
        ProgramID = glCreateProgram();
        glAttachShader(ProgramID, vertex);
        glAttachShader(ProgramID, fragment);
        glLinkProgram(ProgramID);
        CheckCompileErrors(ProgramID, "PROGRAM");

        // Delete the shaders as they're linked into our program now and no longer necessary.
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::Use() const {
        glUseProgram(ProgramID);
    }

    // utility uniform functions
    // ------------------------------------------------------------------------
    void Shader::SetBool(const std::string &Name, bool Value) const {
        glUniform1i(glGetUniformLocation(ProgramID, Name.c_str()), (int) Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetInt(const std::string &Name, int Value) const {
        glUniform1i(glGetUniformLocation(ProgramID, Name.c_str()), Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetFloat(const std::string &Name, float Value) const {
        glUniform1f(glGetUniformLocation(ProgramID, Name.c_str()), Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetUniformMat4(const std::string& Name, const glm::mat4& Value) const {
        glUniformMatrix4fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, GL_FALSE, glm::value_ptr(Value));
    }

    // ------------------------------------------------------------------------
    void Shader::SetVector2(const std::string &Name, const glm::vec2 &Value) const {
        glUniform2f(glGetUniformLocation(ProgramID, Name.c_str()), Value.x, Value.y);
    }

    // ------------------------------------------------------------------------
    void Shader::SetVector3(const std::string &Name, const glm::vec3 &Value) const {
        glUniform3f(glGetUniformLocation(ProgramID, Name.c_str()), Value.x, Value.y, Value.z);
    }

    void Shader::CheckCompileErrors(unsigned int Shader, const std::string &Type) {
        int success;
        char InfoLog[1024];
        if (Type != "PROGRAM") {
            glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(Shader, 1024, NULL, InfoLog);
                spdlog::error("ERROR::SHADER_COMPILATION_ERROR of type: ", Type, "\n", InfoLog);
                Globals::Editor::m_Errors.push_back({"ERROR::SHADER_COMPILATION_ERROR of type: Vertex", InfoLog});
            }
        } else {
            glGetProgramiv(Shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(Shader, 1024, NULL, InfoLog);
                spdlog::error("ERROR::PROGRAM_LINKING_ERROR of type: ", Type, "\n", InfoLog);
                Globals::Editor::m_Errors.push_back({"ERROR::SHADER_COMPILATION_ERROR of type: Fragment", InfoLog});
            }
        }
    }
}