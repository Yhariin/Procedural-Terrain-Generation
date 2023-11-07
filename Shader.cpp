#include "Shader.hpp"

Shader::Shader(const std::string &vertexFile, const std::string &fragFile)
{
    std::string vertexShaderCode = ParseShader(vertexFile);
    std::string fragmentShaderCode = ParseShader(fragFile);

    const char* vertexShaderSourceCode = vertexShaderCode.c_str();
    const char* fragmentShaderSourceCode = fragmentShaderCode.c_str();

    m_ProgramID = CreateShader(vertexShaderSourceCode, fragmentShaderSourceCode);

}

Shader::~Shader()
{
    glDeleteProgram(m_ProgramID);
}

void Shader::Bind() const
{
    glUseProgram(m_ProgramID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

GLuint Shader::getProgramID() const { return m_ProgramID;}


void Shader::setBool(const std::string &name, bool value)
{
    glUniform1i(GetUniformLocation(name), (int)value);
}

void Shader::setInt(const std::string &name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::setFloat(const std::string &name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::setMat4fv(const std::string &name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);

}

int Shader::GetUniformLocation(const std::string &name)
{
    if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_ProgramID, name.c_str());
    if(location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;

}

GLuint Shader::CompileShader(GLuint type, const std::string &source)
{
    GLuint shaderID = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    // TODO: Error handling
    int result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE)
    {
        int length;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        std::cout << "Failed to compile shader!" << std::endl;
        std::cout << message << std::endl;

    }
    
    return shaderID;
}

std::string Shader::ParseShader(const std::string &filename)
{
   std::ifstream stream(filename);

   std::string line;
   std::stringstream ss;

   while(getline(stream, line))
   {
        ss << line << '\n';
   }

   return ss.str();
}

unsigned int Shader::CreateShader(const std::string& vertexShaderSourceCode, const std::string &fragmentShaderSourceCode)
{
    unsigned int program = glCreateProgram();
    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSourceCode);
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSourceCode);

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;

}