#define _CRT_SECURE_NO_WARNINGS
#include "GLMultibrotShaderProgram.h"

GLMultibrotShaderProgram::GLMultibrotShaderProgram(ChaosPanel& canvas)
    : GLShaderProgram(canvas)
{
    GetCanvas()->SetContext();
    BuildVertexShader();
    BuildFragmentShader();
    SetProgramHandle(glCreateProgram());
    glAttachShader(GetProgramHandle(), m_vertexShader->GetShaderHandle());
    glAttachShader(GetProgramHandle(), m_fragmentShader->GetShaderHandle());
    glBindFragDataLocation(GetProgramHandle(), 0, "OutColor");
    Link();
    m_z0 = glGetUniformLocation(GetProgramHandle(), "z0");
    m_p = glGetUniformLocation(GetProgramHandle(), "p");
    m_ul = glGetUniformLocation(GetProgramHandle(), "ul");
    m_lr = glGetUniformLocation(GetProgramHandle(), "lr");
    m_viewDimensions = glGetUniformLocation(GetProgramHandle(), "viewDimensions");
    m_color = glGetUniformLocation(GetProgramHandle(), "color");
}


GLMultibrotShaderProgram::~GLMultibrotShaderProgram()
{
}

void GLMultibrotShaderProgram::BuildVertexShader()
{
    std::string vertexSource =
        "#version 400 core\n"
        "in vec4 position;"
        "void main()"
        "{"
        "    gl_Position = position;"
        "}";
    m_vertexShader = std::make_unique<GLShader>(*GetCanvas(), GL_VERTEX_SHADER, vertexSource,
        "Multibrot vertex shader did not compile.");
}

void GLMultibrotShaderProgram::BuildFragmentShader()
{
    std::string fragmentSource =
        "#version 430 core\n"
        "uniform vec2 z0;"
        "uniform vec2 p;"
        "uniform vec2 ul;"
        "uniform vec2 lr;"
        "uniform vec2 viewDimensions;"
        "uniform vec3 color;"
        "out vec4 OutColor;"
        "vec2 iPower(vec2 vec, vec2 p)"
        "{"
        "    float r = sqrt(vec.x * vec.x + vec.y * vec.y);"
        "    if(r == 0.0f) {"
        "        return vec2(0.0f, 0.0f);"
        "    }"
        "    float theta = vec.y != 0 ? atan(vec.y, vec.x) : (vec.x < 0 ? 3.14159265f : 0.0f);"
        "    float imt = -p.y * theta;"
        "    float rpowr = pow(r, p.x);"
        "    float angle = p.x * theta + p.y * log(r);"
        "    vec2 powr;"
        "    powr.x = rpowr * exp(imt) * cos(angle);"
        "    powr.y = rpowr * exp(imt) * sin(angle);"
        "    return powr;"
        "}"
        ""
        "void main()"
        "{"
        "    vec2 z = z0;"
        "    float x = ul.x + (lr.x - ul.x) * gl_FragCoord.x / (viewDimensions.x - 1);"
        "    float y = ul.y - (ul.y - lr.y) * gl_FragCoord.y / (viewDimensions.y - 1);"
        "    vec2 c = vec2(x, y);"
        "    int i = 0;"
        "    while(z.x * z.x + z.y * z.y < 4.0f && i < 50) {"
        "        z = iPower(z, p) + c;"
        "        ++i;"
        "    }"
        "	 OutColor = i == 50 ? vec4(0.0f, 0.0f, 0.0f, 1.0f) : "
        "        vec4(color * (2.0f * i) / 100.0f, 1.0f);"
        "}";
    m_fragmentShader = std::make_unique<GLShader>(*GetCanvas(), GL_FRAGMENT_SHADER, fragmentSource,
        "Multibrot fragment shader did not compile.");
}
