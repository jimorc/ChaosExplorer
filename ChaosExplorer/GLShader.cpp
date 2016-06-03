#define _CRT_SECURE_NO_WARNINGS
#if !defined(__gl_h_)
#include <GL/glew.h>
#endif
#include "GLShader.h"

GLShader::GLShader(GLenum shaderType,
    const std::string& shaderSource,
    const std::string& compileErrorString)
{
    // build the shader
    m_shader = glCreateShader(shaderType);
    const char *source = shaderSource.c_str();
    glShaderSource(m_shader, 1, &source, NULL);
    glCompileShader(m_shader);
    CheckShaderCompileStatus(m_shader, compileErrorString);
}

GLShader::GLShader(GLShader&& shader)
{
    m_shader = shader.m_shader;
    shader.m_shader = 0;
}


GLShader::~GLShader()
{
    glDeleteShader(m_shader);
}

GLShader& GLShader::operator=(GLShader&& shader)
{
    return std::move(shader);
}

void GLShader::CheckShaderCompileStatus(GLuint shader, const std::string& errorMsg)
{
    // check shader compile status, and throw exception if compile failed
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        throw std::logic_error(errorMsg.c_str());
    }
}

