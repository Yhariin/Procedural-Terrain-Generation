#pragma once

#include <sstream>
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include <glm/mat4x4.hpp>

class Shader
{
    public:

        // Constructor reads and compiles the shader
        Shader(const std::string &vertexFile, const std::string &fragFile);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        GLuint getProgramID() const;

        // Utility uniform functions
        void setBool(const std::string &name, bool value);
        void setInt(const std::string &name, int value);
        void setFloat(const std::string &name, float value);
        void setMat4fv(const std::string &name, const glm::mat4& matrix);
        void setVec3f(const std::string &name, const glm::vec3& vec);

        // void Shader::setUniformMatrix4fv(const std::string &name, glm::mat4x4 mat4x4) const;

    private:
        // Program ID
        unsigned int m_ProgramID;
        std::unordered_map<std::string, int> m_UniformLocationCache;

        unsigned int CompileShader(unsigned int type, const std::string &source);
        std::string ParseShader(const std::string &filename);
        unsigned int CreateShader(const std::string& vertexShaderSourceCode, const std::string &fragmentShaderSourceCode);
        int GetUniformLocation(const std::string &name);
};