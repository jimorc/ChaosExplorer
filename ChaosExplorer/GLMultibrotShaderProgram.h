#pragma once
#include "GLShaderProgram.h"
#include "ChaosPanel.h"

class GLMultibrotShaderProgram :
    public GLShaderProgram
{
public:
    GLMultibrotShaderProgram(ChaosPanel& canvas);
    virtual ~GLMultibrotShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

