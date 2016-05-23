#define _CRT_SECURE_NO_WARNINGS
#include "GLShaderProgram.h"
#include "PlottingCPanel.h"

extern std::vector<glm::vec4> colors;

PlottingCPanel::PlottingCPanel(wxWindow* parent, wxWindowID id, const int* attribList,
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> ul,
    std::complex<float> lr)
    : ChaosPanel(parent, id, attribList, ul, lr, size),
    m_maxIterations(4 * colors.size()),
    m_power(power), m_z0({ 0.0f, 0.0f })
{
}


PlottingCPanel::~PlottingCPanel()
{
}

void PlottingCPanel::DrawFractal(GLShaderProgram* shaderProg)
{
    // draw the image (well, draw the triangles for the display area)
    glUseProgram(shaderProg->GetProgramHandle());
    glBindVertexArray(GetVao());
    glUniform1i(shaderProg->GetUniformHandle("maxIterations"), m_maxIterations);
    glUniform2f(shaderProg->GetUniformHandle("p"), m_power.real(), m_power.imag());
    glUniform2f(shaderProg->GetUniformHandle("z0"), m_z0.real(), m_z0.imag());
    std::complex<float> upperLeft = GetUpperLeft();
    std::complex<float> lowerRight = GetLowerRight();
    glUniform2f(shaderProg->GetUniformHandle("ul"), upperLeft.real(), upperLeft.imag());
    glUniform2f(shaderProg->GetUniformHandle("lr"), lowerRight.real(), lowerRight.imag());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}