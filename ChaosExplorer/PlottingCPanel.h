#pragma once
#include "wx/wxprec.h"
#include "ChaosPanel.h"
#include <complex>

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
};

