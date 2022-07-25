// Copyright (c) 2022.

#include <glad/glad.h>
#include <sstream>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>
#include "shader.hpp"
#include "../../../util/filehandler/filehandler.hpp"
#include "../../../util/globals/global.hpp"

namespace Viper::Graphics {

    // Shader Commands
    void ShaderCommand::UploadMat4( uint32_t ProgramID, const std::string& Name, const glm::mat4& Value ) {
        glUniformMatrix4fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, GL_FALSE, glm::value_ptr(Value));
    };

    void ShaderCommand::UploadVec2( uint32_t ProgramID, const std::string& Name, const glm::vec2& Value ) {
        glUniform2fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, glm::value_ptr(Value));
    };

    void ShaderCommand::UploadVec3( uint32_t ProgramID, const std::string& Name, const glm::vec3& Value ) {
        glUniform3fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, glm::value_ptr(Value));
    };

    void ShaderCommand::UploadVec4( uint32_t ProgramID, const std::string& Name, const glm::vec4& Value ) {
        glUniform4fv(glGetUniformLocation(ProgramID, Name.c_str()), 1, glm::value_ptr(Value));
    };

    void ShaderCommand::UploadInt( uint32_t ProgramID, const std::string& Name, int Value ) {
        glUniform1i(glGetUniformLocation(ProgramID, Name.c_str()), Value);
    };

    void ShaderCommand::UploadFloat( uint32_t ProgramID, const std::string& Name, float Value ) {
        glUniform1f(glGetUniformLocation(ProgramID, Name.c_str()), Value);
    };

    void ShaderCommand::UploadBool( uint32_t ProgramID, const std::string& Name, bool Value ) {
        glUniform1i(glGetUniformLocation(ProgramID, Name.c_str()), static_cast< int >( Value ));
    };

    void ShaderCommand::UploadDouble( uint32_t ProgramID, const std::string& Name, double Value ) {
        glUniform1d(glGetUniformLocation(ProgramID, Name.c_str()), Value);
    };

    // Shader Class
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
        ShaderCommand::UploadBool(ProgramID, Name, Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetInt(const std::string &Name, int Value) const {
         ShaderCommand::UploadInt(ProgramID, Name, Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetFloat(const std::string &Name, float Value) const {
        ShaderCommand::UploadFloat(ProgramID, Name, Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetDouble(const std::string &Name, double Value) const {
        ShaderCommand::UploadDouble(ProgramID, Name, Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetUniformMat4(const std::string& Name, const glm::mat4& Value) const {
        ShaderCommand::UploadMat4(ProgramID, Name, Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetVector2(const std::string &Name, const glm::vec2 &Value) const {
        ShaderCommand::UploadVec2(ProgramID, Name, Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetVector3(const std::string &Name, const glm::vec3 &Value) const {
        ShaderCommand::UploadVec3(ProgramID, Name, Value);
    }

    // ------------------------------------------------------------------------
    void Shader::SetVector4(const std::string &Name, const glm::vec4 &Value) const {
        ShaderCommand::UploadVec4(ProgramID, Name, Value);
    }

    void Shader::CheckCompileErrors(unsigned int Shader, const std::string &Type) {
        int success;
        char InfoLog[1024];
        if (Type != "PROGRAM") {
            glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(Shader, 1024, NULL, InfoLog);
                spdlog::error("ERROR::SHADER_COMPILATION_ERROR of type: ", Type, "\n", InfoLog);
                Globals::ConsoleContext::AddLog( "ERROR::SHADER_COMPILATION_ERROR!", InfoLog );
            }
        } else {
            glGetProgramiv(Shader, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(Shader, 1024, NULL, InfoLog);
                spdlog::error("ERROR::PROGRAM_LINKING_ERROR of type: ", Type, "\n", InfoLog);
                Globals::ConsoleContext::AddLog( "ERROR::SHADER_COMPILATION_ERROR!", InfoLog );
            }
        }
    }
}