#pragma once
#include "GLShaderProgram.h"

class GLMultibrotShaderProgram :
    public GLShaderProgram
{
public:
    GLMultibrotShaderProgram();
    virtual ~GLMultibrotShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

