#define _CRT_SECURE_NO_WARNINGS
#include "GLFractal2JuliaShaderProgram.h"

GLFractal2JuliaShaderProgram::GLFractal2JuliaShaderProgram()
    : GLShaderProgram()
{
    // build and link the Multibrot shaders and program
    BuildVertexShader();
    BuildFragmentShader();
    SetProgramHandle(glCreateProgram());
    glAttachShader(GetProgramHandle(), GetVertexShader()->GetShaderHandle());
    glAttachShader(GetProgramHandle(), m_fragmentShader->GetShaderHandle());
    glBindFragDataLocation(GetProgramHandle(), 0, "OutColor");
    Link();
    // get the handles for the various uniforms
    LoadUniformHandles();
}


GLFractal2JuliaShaderProgram::~GLFractal2JuliaShaderProgram()
{
}

void GLFractal2JuliaShaderProgram::BuildFragmentShader()
{
    // iPower calculates the complex power of a complex number.
    // Equation given in http://mathworld.wolfram.com/ComplexExponentiation.html
    // Similar to the code in http://abecedarical.com/zenosamples/zs_complexnumbers.html
    // GLSL does not have complex number support and exp/log and trig functions are limited
    // to floats.
    //
    // iDivide calculates the division of one complex number by another.
    // Equation given in http://mathworld.wolfram.com/ComplexDivision.html.
    std::string fragmentSource =
        "#version 330 core\n"
        "uniform vec2 c;"
        "uniform vec2 p;"
        "uniform vec2 ul;"
        "uniform vec2 lr;"
        "uniform vec2 viewDimensions;"
        "uniform int maxIterations;"
        "uniform vec4 color[50];"
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
        "vec2 iDivide(vec2 v1, vec2 v2)"
        "{"
        "    vec2 v;"
        "    float v2squared = v2.x * v2.x + v2.y * v2.y;"
        "    v.x = (v1.x * v2.x + v1.y * v2.y) / v2squared;"
        "    v.y = (v1.y * v2.x - v1.x * v2.y) / v2squared;"
        "    return v;"
        "}"
        ""
        "vec2 iMultiply(vec2 v1, vec2 v2)"
        "{"
        "    vec2 v;"
        "    v.x = v1.x * v2.x - v1.y * v2.y;"
        "    v.y = v1.x * v2.y + v1.y * v2.x;"
        "    return v;"
        "}"
        ""
        "void main()"
        "{"
        "    float x = ul.x + (lr.x - ul.x) * gl_FragCoord.x / (viewDimensions.x - 1);"
        "    float y = lr.y + (ul.y - lr.y) * gl_FragCoord.y / (viewDimensions.y - 1);"
        "    vec2 z = vec2(x, y);"
        "    int i = 0;"
        "    while(z.x * z.x + z.y * z.y < 4.0f && i < 200) {"
        "        vec2 numer = iPower(z, vec2(3.0f, 0.0f)) + 1.0f;"
        "        vec2 denom = 1.0f + iMultiply(c, iPower(z, vec2(2.0f, 0.0f)));"
        "        z = iDivide(numer, denom);"
        "        ++i;"
        "    }"
        "	 OutColor = i == maxIterations ? vec4(0.0f, 0.0f, 0.0f, 1.0f) : "
        "        color[i%50 - 1];"
        "}";
    m_fragmentShader = std::make_unique<GLShader>(GL_FRAGMENT_SHADER, fragmentSource,
        "Fractal2 fragment shader did not compile.");
}
