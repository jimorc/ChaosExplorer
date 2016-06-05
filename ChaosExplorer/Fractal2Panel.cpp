#include "Fractal2Panel.h"

extern std::vector<glm::vec4> colors;

Fractal2Panel::Fractal2Panel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> ul,
    std::complex<float> lr)
    : PlottingCPanel<GLFractal2ShaderProgram, MandelJuliaPanel>(parent, id, attribList, size, power, ul, lr)
{
    // set up GL stuff
    CreateShaderProgram();
    SetupTriangles();
    SetupSquareArrays();
    glUseProgram(GetShaderProgram()->GetProgramHandle());
    GLFractal2ShaderProgram* prog = dynamic_cast<GLFractal2ShaderProgram*>(GetShaderProgram());
    glUniform2f(prog->GetUniformHandle("viewDimensions"), size.x, size.y);
    glUniform4fv(prog->GetUniformHandle("color[0]"), colors.size() * 4, &colors[0].x);
}


Fractal2Panel::~Fractal2Panel()
{
}
