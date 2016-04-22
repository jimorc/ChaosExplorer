#define _CRT_SECURE_NO_WARNINGS
#include "GLShader.h"

GLShader::GLShader(MultibrotPanel& canvas, GLenum shaderType,
    const std::string& shaderSource,
    const std::string& compileErrorString)
    : m_canvas(&canvas)
{
    m_canvas->SetCurrent(*m_canvas->GetContext());
    m_shader = glCreateShader(GL_VERTEX_SHADER);
    const char *source = shaderSource.c_str();
    glShaderSource(m_shader, 1, &source, NULL);
    glCompileShader(m_shader);
    CheckShaderCompileStatus(m_shader, "Circle Vertex Shader did not compile.");
}


GLShader::~GLShader() noexcept
{
    m_canvas->SetCurrent(*m_canvas->GetContext());
    glDeleteShader(m_shader);
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

