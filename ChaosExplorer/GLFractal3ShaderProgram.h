#pragma once
#include "GLShaderProgram.h"

class GLFractal3ShaderProgram :
    public GLShaderProgram
{
public:
    GLFractal3ShaderProgram();
    virtual ~GLFractal3ShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

