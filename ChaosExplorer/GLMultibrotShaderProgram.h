#pragma once
#include "GLShaderProgram.h"
#include "ChaosPanel.h"

class GLMultibrotShaderProgram :
    public GLShaderProgram
{
public:
    GLMultibrotShaderProgram(ChaosPanel& canvas);
    virtual ~GLMultibrotShaderProgram();

    GLint GetZ0Handle() { return m_z0; }
    GLint GetPHandle() { return m_p; }
    GLint GetULHandle() { return m_ul; }
    GLint GetLRHandle() { return m_lr; }
    GLint GetViewDimensionsHandle() { return m_viewDimensions; }
    GLint GetColorHandle() { return m_color; }
    GLint GetMaxIterationsHandle() { return m_maxIterations; }

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
    GLint m_z0;
    GLint m_p;
    GLint m_ul;
    GLint m_lr;
    GLint m_viewDimensions;
    GLint m_color;
    GLint m_maxIterations;
};

