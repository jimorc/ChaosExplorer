#pragma once
#include "wx/wxprec.h"
#include "GLShaderProgram.h"
#include "ChaosPanel.h"
#include <complex>

using namespace std::complex_literals;

template <typename T>
class PlottingCPanel :
    public ChaosPanel<T>
{
public:
    PlottingCPanel<T>(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> ul = -2.5f + 2.0if,
        std::complex<float> lr = 1.5f - 2.0if)
        : ChaosPanel<T>(parent, id, attribList, ul, lr, size),
        m_maxIterations(4 * colors.size()),
        m_power(power), m_z0({ 0.0f, 0.0f })
    {
    }

    virtual ~PlottingCPanel() {}

protected:
    virtual void DrawFractal(T* shaderProg) override
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

    void SetMaxIterations(const int iterations) { m_maxIterations = iterations; }
    void IncrementMaxIterations() { ++m_maxIterations; }
    int GetMaxIterations() { return m_maxIterations; }
    void SetZ0(const std::complex<float>& z0) { m_z0 = z0; }
    std::complex<float> GetZ0() { return m_z0; }
    void SetPower(const std::complex<float>& power) { m_power = power; }
    std::complex<float> GetPower() const { return m_power; }


private:
    int m_maxIterations;
    std::complex<float> m_z0;
    std::complex<float> m_power;
};

