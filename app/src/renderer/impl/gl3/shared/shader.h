#pragma once
#include <cstdint>
#include <glad/gl.h>

namespace nt
{
    namespace GL3
    {
        GLuint CompileVertexShader(const char *shaderSource);
        GLuint CompileFragmentShader(const char *shaderSource);
        GLuint LinkShaders(GLuint vertex, GLuint fragment);
        GLuint CompileAndLinkShaders(const char *vertexShaderSource,
                                     const char *FragmentShaderSource);
    } // namespace GL3
} // namespace nt