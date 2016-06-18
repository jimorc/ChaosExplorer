#pragma once
#include "GLShaderProgram.h"

class GLFractal4JuliaShaderProgram :
    public GLShaderProgram
{
public:
    GLFractal4JuliaShaderProgram();
    virtual ~GLFractal4JuliaShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

