#pragma once
#include "GLShaderProgram.h"

class GLFractal4ShaderProgram :
    public GLShaderProgram
{
public:
    GLFractal4ShaderProgram();
    virtual ~GLFractal4ShaderProgram();

private:
    void BuildFragmentShader();

    std::unique_ptr<GLShader> m_fragmentShader;
};

