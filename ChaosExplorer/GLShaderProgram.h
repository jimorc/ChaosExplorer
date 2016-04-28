#pragma once
#include <string>
#include <vector>
#include "GL/glew.h"
#include <string>
#include <vector>
#include "GLShader.h"
#include "ChaosPanel.h"

class GLShaderProgram
{
public:
    GLShaderProgram(ChaosPanel& canvas);
    GLShaderProgram(const GLShaderProgram&) = delete;
    GLShaderProgram(GLShaderProgram&&) = delete;
    virtual ~GLShaderProgram();
    GLShaderProgram& operator=(const GLShaderProgram) = delete;
    GLShaderProgram& operator=(GLShaderProgram&&) = delete;
    void Link();
    void Use() { glUseProgram(m_program); }
    GLuint GetProgramHandle() { return m_program; }
    GLint GetUniformLocation(std::string& uniform) { glGetUniformLocation(m_program, uniform.c_str()); }
    GLint GetZ0Handle() { return m_z0; }
    GLint GetPHandle() { return m_p; }
    GLint GetULHandle() { return m_ul; }
    GLint GetLRHandle() { return m_lr; }
    GLint GetViewDimensionsHandle() { return m_viewDimensions; }
    GLint GetColorHandle() { return m_color; }

private:
    void BuildVertexShader();
    void BuildFragmentShader();

    ChaosPanel* m_canvas;
    GLuint m_program;
    std::unique_ptr<GLShader> m_vertexShader;
    std::unique_ptr<GLShader> m_fragmentShader;

    GLint m_z0;
    GLint m_p;
    GLint m_ul;
    GLint m_lr;
    GLint m_viewDimensions;
    GLint m_color;
};

