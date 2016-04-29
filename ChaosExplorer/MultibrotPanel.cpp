#define _CRT_SECURE_NO_WARNINGS
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "GL/glew.h"
#include "GLMultibrotShaderProgram.h"
#include "GLSquareShaderProgram.h"
#include "MultibrotPanel.h"


MultibrotPanel::MultibrotPanel(wxWindow* parent, wxWindowID id, const int* attribList, 
    const wxSize& size,
    std::complex<float> power,
    std::complex<float> ul,
    std::complex<float> lr)
    : ChaosPanel(parent, id, attribList, size),
    m_power(power), m_leftButtonDown(false), m_leftDown({ 0, 0 }), m_leftUp({ 0, 0 })
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
    Bind(wxEVT_LEFT_DOWN, &MultibrotPanel::OnLeftButtonDown, this);
    Bind(wxEVT_LEFT_UP, &MultibrotPanel::OnLeftButtonUp, this);
    Bind(wxEVT_MOTION, &MultibrotPanel::OnMouseMove, this);

    std::vector<glm::vec4> vertices;
    vertices.push_back({ 1.0f, 1.0f, 0.0f, 1.0f });
    vertices.push_back({ -1.0f, 1.0f, 0.0f, 1.0f });
    vertices.push_back({ 1.0f, -1.0f, 0.0f, 1.0f });
    vertices.push_back({ -1.0f, -1.0f, 0.0f, 1.0f });
    BuildShaderProgram();
    SetupTriangles(vertices, m_program->GetProgramHandle());
    SetupSquareArrays();
}


MultibrotPanel::~MultibrotPanel()
{
    glDeleteBuffers(1, &m_squareVbo);
    glDeleteVertexArrays(1, &m_squareVao);
}

void MultibrotPanel::OnPaint(wxPaintEvent& event)
{
    wxSize size = GetSize();
    SetContext();
    // set background to black
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_program->GetProgramHandle());
    glBindVertexArray(GetVao());
    GLMultibrotShaderProgram* prog = dynamic_cast<GLMultibrotShaderProgram*>(m_program.get());
    glUniform2f(prog->GetZ0Handle(), 0.0f, 0.0f);
    glUniform2f(prog->GetPHandle(), m_power.real(), m_power.imag());
    glUniform2f(prog->GetULHandle(), m_upperLeft.real(), m_upperLeft.imag());
    glUniform2f(prog->GetLRHandle(), m_lowerRight.real(), m_lowerRight.imag());
    glUniform2f(prog->GetViewDimensionsHandle(), size.x, size.y);
    glUniform3f(prog->GetColorHandle(), 1.0f, 0.5f, 0.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    if (m_leftDown.x != m_leftUp.x || m_leftDown.y != m_leftUp.y) {
        glUseProgram(m_squareProgram->GetProgramHandle());
        glBindVertexArray(m_squareVao);
        float halfSize = static_cast<float>(size.x) / 2.0f;
        float downX = m_leftDown.x - halfSize;
        float downY = halfSize - m_leftDown.y;
        float upX = m_leftUp.x - halfSize;
        float upY = halfSize - m_leftUp.y;
        std::vector<glm::vec4> points;
        points.push_back({ downX, downY, 0.0f, halfSize });
        points.push_back({ downX, upY, 0.0f, halfSize });
        points.push_back({ upX, upY, 0.0f, halfSize });
        points.push_back({ upX, downY, 0.0f, halfSize });
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(points[0]), &points[0], GL_DYNAMIC_DRAW);
        glDrawArrays(GL_LINE_LOOP, 0, points.size());
    }

    glFlush();
    SwapBuffers();
}


void MultibrotPanel::BuildShaderProgram()
{
    m_program = std::make_unique<GLMultibrotShaderProgram>(*this);
    m_squareProgram = std::make_unique<GLSquareShaderProgram>(*this);
}

void MultibrotPanel::OnLeftButtonDown(wxMouseEvent& event)
{
    m_leftButtonDown = true;
    m_leftDown = event.GetPosition();
    m_leftUp = m_leftDown;
}

void MultibrotPanel::OnMouseMove(wxMouseEvent& event)
{
    if (m_leftButtonDown) {
        m_leftUp = event.GetPosition();
        if (m_leftDown.x > m_leftUp.x) {
            int temp = m_leftDown.x;
            m_leftDown.x = m_leftUp.x;
            m_leftUp.x = temp;
        }
        if (m_leftDown.y > m_leftUp.y) {
            m_leftDown.y = m_leftUp.y;
        }
        m_leftUp.y = m_leftDown.y + (m_leftUp.x - m_leftDown.x);
        Refresh();
    }
}

void MultibrotPanel::OnLeftButtonUp(wxMouseEvent& event)
{
    m_leftUp = event.GetPosition();
    m_leftButtonDown = false;
    if (m_leftDown.x > m_leftUp.x) {
        int temp = m_leftDown.x;
        m_leftDown.x = m_leftUp.x;
        m_leftUp.x = temp;
    }
    if (m_leftDown.y > m_leftUp.y) {
        m_leftDown.y = m_leftUp.y;
    }
    m_leftUp.y = m_leftDown.y + (m_leftUp.x - m_leftDown.x);
    Refresh();
}

void MultibrotPanel::SetupSquareArrays()
{
    glGenVertexArrays(1, &m_squareVao);
    glBindVertexArray(m_squareVao);
    glGenBuffers(1, &m_squareVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_squareVbo);
    GLint posAttrib = glGetAttribLocation(m_squareProgram->GetProgramHandle(), "position");
    glVertexAttribPointer(posAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(posAttrib);

}
