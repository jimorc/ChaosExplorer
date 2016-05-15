#pragma once
#include "GLShaderProgram.h"
#include "ChaosPanel.h"

class GLMandelJuliaShaderProgram :
    public GLShaderProgram
{
public:
    GLMandelJuliaShaderProgram(ChaosPanel& canvas);
    virtual ~GLMandelJuliaShaderProgram();

    GLint GetCHandle() { return m_c; }
    GLint GetPHandle() { return m_p; }
    GLint GetULHandle() { return m_ul; }
    GLint GetLRHandle() { return m_lr; }
    GLint GetViewDimensionsHandle() { return m_viewDimensions; }
    GLint GetColorHandle() { return m_color; }

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
    GLint m_c;
    GLint m_p;
    GLint m_ul;
    GLint m_lr;
    GLint m_viewDimensions;
    GLint m_color;
};

