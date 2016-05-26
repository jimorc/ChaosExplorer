#pragma once
#include "GLShaderProgram.h"
#include "ChaosPanel.h"

class GLMandelJuliaShaderProgram :
    public GLShaderProgram
{
public:
    GLMandelJuliaShaderProgram(ChaosPanel& canvas);
    virtual ~GLMandelJuliaShaderProgram();

private:
    void BuildFragmentShader(ChaosPanel& canvas);

    std::unique_ptr<GLShader> m_fragmentShader;
};

