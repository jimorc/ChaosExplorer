#define _CRT_SECURE_NO_WARNINGS
#include "GLSquareShaderProgram.h"

GLSquareShaderProgram::GLSquareShaderProgram()
    : GLShaderProgram()
{
    // build and link the square shaders and program
    BuildVertexShader();
    BuildFragmentShader();
    SetProgramHandle(glCreateProgram());
    glAttachShader(GetProgramHandle(),GetVertexShader()->GetShaderHandle());
    glAttachShader(GetProgramHandle(), m_fragmentShader->GetShaderHandle());
    glBindFragDataLocation(GetProgramHandle(), 0, "OutColor");
    Link();
}


GLSquareShaderProgram::~GLSquareShaderProgram()
{
}

void GLSquareShaderProgram::BuildFragmentShader()
{
    std::string fragmentSource =
        "#version 330 core\n"
        "out vec4 OutColor;"
        "void main()"
        "{"
        "    OutColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
        "}";
    m_fragmentShader = std::make_unique<GLShader>(GL_FRAGMENT_SHADER, fragmentSource,
        "Square fragment shader did not compile.");
}
