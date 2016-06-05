#pragma once
#include "GLShaderProgram.h"

class GLFractal2JuliaShaderProgram :
    public GLShaderProgram
{
public:
    GLFractal2JuliaShaderProgram();
    virtual ~GLFractal2JuliaShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};



