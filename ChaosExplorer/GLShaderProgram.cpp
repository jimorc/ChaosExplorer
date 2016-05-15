#define _CRT_SECURE_NO_WARNINGS
#include "GLShaderProgram.h"

GLShaderProgram::GLShaderProgram(ChaosPanel& canvas) 
    : m_canvas(&canvas)
{
}


GLShaderProgram::~GLShaderProgram()
{
    glDeleteProgram(m_program);
}


void GLShaderProgram::Link()
{
    glLinkProgram(m_program);
    // check shader compile status, and throw exception if compile failed
    GLint status = GL_TRUE;
    glGetShaderiv(m_program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        // Program Info Log is empty
        // so don't try to read it
        throw std::logic_error("Linking of shader program failed.");
    }

}

// Vertex shader is common among the shader programs.
// It can, however, be overridden if necessary.
void GLShaderProgram::BuildVertexShader()
{
    std::string vertexSource =
        "#version 330 core\n"
        "in vec4 position;"
        "void main()"
        "{"
        "    gl_Position = position;"
        "}";
    m_vertexShader = std::make_unique<GLShader>(*GetCanvas(), GL_VERTEX_SHADER, vertexSource,
        "Vertex shader did not compile.");
}
