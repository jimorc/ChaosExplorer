#pragma once
#include "wx/wxprec.h"
#include "ChaosPanel.h"
#include <complex>

class GLShaderProgram;      // forward declaration

using namespace std::complex_literals;

class PlottingCPanel :
    public ChaosPanel
{
public:
    PlottingCPanel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<float> power = 2.0f,
        std::complex<float> ul = -2.5f + 2.0if,
        std::complex<float> lr = 1.5f - 2.0if);
    virtual ~PlottingCPanel();

protected:
    void DrawFractal(GLShaderProgram* shaderProg);

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

