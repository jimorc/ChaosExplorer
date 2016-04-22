#pragma once
#include <complex>
#include "wx/panel.h"

using namespace std::complex_literals;

class MultibrotPanel :
    public wxPanel
{
public:
    MultibrotPanel(wxWindow* parent, wxWindowID id, const wxSize& size = { 800, 600 },
        std::complex<double> power = { 2.0L, 0.0L },
        std::complex<double> ul = -2.5 + 2.0i,
        std::complex<double> lr =  1.5 - 2.0i);
    virtual ~MultibrotPanel();
private:
    std::complex<double> m_power;
    std::complex<double> m_upperLeft;
    std::complex<double> m_lowerRight;

};

