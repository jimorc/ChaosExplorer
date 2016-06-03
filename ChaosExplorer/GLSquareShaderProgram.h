#pragma once
#include <wx/wxprec.h>
#include "GLShaderProgram.h"

class GLSquareShaderProgram :
    public GLShaderProgram
{
public:
    GLSquareShaderProgram();
    virtual ~GLSquareShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

