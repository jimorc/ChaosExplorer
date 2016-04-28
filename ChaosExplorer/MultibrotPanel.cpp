#define _CRT_SECURE_NO_WARNINGS
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "GLMultibrotShaderProgram.h"
#include "MultibrotPanel.h"


MultibrotPanel::MultibrotPanel(wxWindow* parent, wxWindowID id, const int* attribList, 
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> ul,
    std::complex<float> lr)
    : ChaosPanel(parent, id, attribList, size),
    m_power(power)
{
    if (power.real() < 1.0f) {
        throw std::invalid_argument(
            "The real portion of the power argument for Multibrot is less than 1.0");
    }
    m_upperLeft = std::complex<float>(std::min(ul.real(), lr.real()), std::max(ul.imag(), lr.imag()));
    m_lowerRight = std::complex<float>(std::max(ul.real(), lr.real()), std::min(ul.imag(), lr.imag()));
    if (ul.real() == lr.real()) {
        throw std::invalid_argument(
            "The real portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }
    if (ul.imag() == lr.imag()) {
        throw std::invalid_argument(
            "The imag portions of the upper-left and lower-right corners of the Multibrot display are equal.");
    }

    Bind(wxEVT_PAINT, &MultibrotPanel::OnPaint, this);

    std::vector<glm::vec4> vertices;
    vertices.push_back({ 1.0f, 1.0f, 0.0f, 1.0f });
    vertices.push_back({ -1.0f, 1.0f, 0.0f, 1.0f });
    vertices.push_back({ 1.0f, -1.0f, 0.0f, 1.0f });
    vertices.push_back({ -1.0f, -1.0f, 0.0f, 1.0f });
    BuildShaderProgram();
    SetupTriangles(vertices, m_program->GetProgramHandle());
}


MultibrotPanel::~MultibrotPanel()
{
}

void MultibrotPanel::OnPaint(wxPaintEvent& event)
{
    wxSize size = GetSize();
    SetContext();
    // set background to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_program->GetProgramHandle());
    GLMultibrotShaderProgram* prog = dynamic_cast<GLMultibrotShaderProgram*>(m_program.get());
    glUniform2f(prog->GetZ0Handle(), 0.0f, 0.0f);
    glUniform2f(prog->GetPHandle(), m_power.real(), m_power.imag());
    glUniform2f(prog->GetULHandle(), m_upperLeft.real(), m_upperLeft.imag());
    glUniform2f(prog->GetLRHandle(), m_lowerRight.real(), m_lowerRight.imag());
    glUniform2f(prog->GetViewDimensionsHandle(), size.x, size.y);
    glUniform3f(prog->GetColorHandle(), 1.0f, 0.5f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glFlush();
    SwapBuffers();
}


void MultibrotPanel::BuildShaderProgram()
{
    m_program = std::make_unique<GLMultibrotShaderProgram>(*this);
}
