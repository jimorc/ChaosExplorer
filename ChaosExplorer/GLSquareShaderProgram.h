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
    void BuildFragmentShader(ChaosPanel& canvas);

    std::unique_ptr<GLShader> m_fragmentShader;
};

