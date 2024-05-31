// #define GL_GLEXT_PROTOTYPES
#include "shader.h"
// #include <glad/gl.h>

#include <sstream>
#include <stdexcept>

GLuint CompileShader(const char *shaderSource, GLuint Type)
{
    // Build and compile our shader program
    GLuint vertexShader = glCreateShader(Type);
    glShaderSource(vertexShader, 1, &shaderSource, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::stringstream ss;

        ss << "ERROR::SHADER::";

        switch (Type)
        {
        case GL_VERTEX_SHADER:
            ss << "VERTEX";
            break;
        case GL_FRAGMENT_SHADER:
            ss << "FRAGMENT";
            break;
        }

        ss << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::runtime_error(ss.str().c_str());
    }
    return vertexShader;
}

GLuint nt::GL3::CompileVertexShader(const char *shaderSource)
{
    return CompileShader(shaderSource, GL_VERTEX_SHADER);
}

GLuint nt::GL3::CompileFragmentShader(const char *shaderSource)
{
    return CompileShader(shaderSource, GL_FRAGMENT_SHADER);
}

GLuint nt::GL3::CompileAndLinkShaders(const char *vertexShaderSource,
                                      const char *FragmentShaderSource)
{
    GLuint vertex = CompileVertexShader(vertexShaderSource);
    GLuint fragment = CompileFragmentShader(FragmentShaderSource);
    GLuint linked = LinkShaders(vertex, fragment);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return linked;
}

GLuint nt::GL3::LinkShaders(GLuint vertex, GLuint fragment)
{
    GLint success;
    GLchar infoLog[512];
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::stringstream ss;

        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        ss << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
           << infoLog << std::endl;
        throw std::runtime_error(ss.str().c_str());
    }

    return shaderProgram;
}
