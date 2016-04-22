#pragma once
#include <complex>
#include <memory>
#include "wx/panel.h"
#include "wx/glcanvas.h"

using namespace std::complex_literals;

class MultibrotPanel :
    public wxGLCanvas
{
public:
    MultibrotPanel(wxWindow* parent, wxWindowID id, const int* attribList,
        const wxSize& size = { 800, 600 },
        std::complex<double> power = { 2.0L, 0.0L },
        std::complex<double> ul = -2.5 + 2.0i,
        std::complex<double> lr =  1.5 - 2.0i);
    virtual ~MultibrotPanel();
private:
    void InitializeGLEW();
    std::complex<double> m_power;
    std::complex<double> m_upperLeft;
    std::complex<double> m_lowerRight;

    std::unique_ptr<wxGLContext> m_context;
};

