#pragma once
#include <complex>
#include <memory>
#include "wx/panel.h"
#include "ChaosPanel.h"

using namespace std::complex_literals;

class MultibrotPanel :
    public ChaosPanel
{
public:
    MultibrotPanel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<double> power = { 2.0L, 0.0L },
        std::complex<double> ul = -2.5 + 2.0i,
        std::complex<double> lr =  1.5 - 2.0i);
    virtual ~MultibrotPanel();

protected:
    virtual void BuildVertexShader() override;
    virtual void BuildFragmentShader() override;
    virtual void BuildShaderProgram() override;

private:
    virtual void OnPaint(wxPaintEvent& event) override;

    std::complex<double> m_power;
    std::complex<double> m_upperLeft;
    std::complex<double> m_lowerRight;
};

