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


