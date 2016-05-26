#define _CRT_SECURE_NO_WARNINGS
#include "GLSquareShaderProgram.h"



GLSquareShaderProgram::GLSquareShaderProgram(ChaosPanel& canvas)
    : GLShaderProgram(canvas)
{
    // build and link the square shaders and program
    canvas.SetContext();
    BuildVertexShader(canvas);
    BuildFragmentShader(canvas);
    SetProgramHandle(glCreateProgram());
    glAttachShader(GetProgramHandle(),GetVertexShader()->GetShaderHandle());
    glAttachShader(GetProgramHandle(), m_fragmentShader->GetShaderHandle());
    glBindFragDataLocation(GetProgramHandle(), 0, "OutColor");
    Link();
}


GLSquareShaderProgram::~GLSquareShaderProgram()
{
}

void GLSquareShaderProgram::BuildFragmentShader(ChaosPanel& canvas)
{
    std::string fragmentSource =
        "#version 330 core\n"
        "out vec4 OutColor;"
        "void main()"
        "{"
        "    OutColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
        "}";
    m_fragmentShader = std::make_unique<GLShader>(canvas, GL_FRAGMENT_SHADER, fragmentSource,
        "Square fragment shader did not compile.");
}
