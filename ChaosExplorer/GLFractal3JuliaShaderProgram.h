#pragma once
#include "GLShaderProgram.h"

class GLFractal3JuliaShaderProgram :
    public GLShaderProgram
{
public:
    GLFractal3JuliaShaderProgram();
    virtual ~GLFractal3JuliaShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

