#pragma once
#include "GLShaderProgram.h"

class GLMandelJuliaShaderProgram :
    public GLShaderProgram
{
public:
    GLMandelJuliaShaderProgram();
    virtual ~GLMandelJuliaShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

