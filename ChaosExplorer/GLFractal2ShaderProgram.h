#pragma once
#include "GLShaderProgram.h"

class GLFractal2ShaderProgram :
    public GLShaderProgram
{
public:
    GLFractal2ShaderProgram();
    virtual ~GLFractal2ShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

