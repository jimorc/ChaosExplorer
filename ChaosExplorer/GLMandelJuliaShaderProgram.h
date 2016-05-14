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
    GLint GetViewDimensionsHandle() { return m_viewDimensions; }
    GLint GetColorHandle() { return m_color; }

private:
    void BuildVertexShader();
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_vertexShader;
    std::unique_ptr<GLShader> m_fragmentShader;
    GLint m_c;
    GLint m_p;
    GLint m_viewDimensions;
    GLint m_color;
};

