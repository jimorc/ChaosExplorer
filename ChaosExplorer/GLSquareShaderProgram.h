#pragma once
#include "GLShaderProgram.h"
#include "ChaosPanel.h"

class GLSquareShaderProgram :
    public GLShaderProgram
{
public:
    GLSquareShaderProgram(ChaosPanel& canvas);
    virtual ~GLSquareShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

