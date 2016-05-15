#define _CRT_SECURE_NO_WARNINGS
#include "GLSquareShaderProgram.h"



GLSquareShaderProgram::GLSquareShaderProgram(ChaosPanel& canvas)
    : GLShaderProgram(canvas)
{
    // build and link the square shaders and program
    GetCanvas()->SetContext();
    BuildVertexShader();
    BuildFragmentShader();
    SetProgramHandle(glCreateProgram());
    glAttachShader(GetProgramHandle(), m_vertexShader->GetShaderHandle());
    glAttachShader(GetProgramHandle(), m_fragmentShader->GetShaderHandle());
    glBindFragDataLocation(GetProgramHandle(), 0, "OutColor");
    Link();
}


GLSquareShaderProgram::~GLSquareShaderProgram()
{
}

void GLSquareShaderProgram::BuildVertexShader()
{
    std::string vertexSource =
        "#version 330 core\n"
        "in vec4 position;"
        "void main()"
        "{"
        "    gl_Position = position;"
        "}";
    m_vertexShader = std::make_unique<GLShader>(*GetCanvas(), GL_VERTEX_SHADER, vertexSource,
        "Square vertex shader did not compile.");

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
    m_fragmentShader = std::make_unique<GLShader>(*GetCanvas(), GL_FRAGMENT_SHADER, fragmentSource,
        "Square fragment shader did not compile.");
}
